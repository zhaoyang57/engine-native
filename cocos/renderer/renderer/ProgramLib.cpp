/****************************************************************************
 LICENSING AGREEMENT
 
 Xiamen Yaji Software Co., Ltd., (the “Licensor”) grants the user (the “Licensee”) non-exclusive and non-transferable rights to use the software according to the following conditions:
 a.  The Licensee shall pay royalties to the Licensor, and the amount of those royalties and the payment method are subject to separate negotiations between the parties.
 b.  The software is licensed for use rather than sold, and the Licensor reserves all rights over the software that are not expressly granted (whether by implication, reservation or prohibition).
 c.  The open source codes contained in the software are subject to the MIT Open Source Licensing Agreement (see the attached for the details);
 d.  The Licensee acknowledges and consents to the possibility that errors may occur during the operation of the software for one or more technical reasons, and the Licensee shall take precautions and prepare remedies for such events. In such circumstance, the Licensor shall provide software patches or updates according to the agreement between the two parties. The Licensor will not assume any liability beyond the explicit wording of this Licensing Agreement.
 e.  Where the Licensor must assume liability for the software according to relevant laws, the Licensor’s entire liability is limited to the annual royalty payable by the Licensee.
 f.  The Licensor owns the portions listed in the root directory and subdirectory (if any) in the software and enjoys the intellectual property rights over those portions. As for the portions owned by the Licensor, the Licensee shall not:
 - i. Bypass or avoid any relevant technical protection measures in the products or services;
 - ii. Release the source codes to any other parties;
 - iii. Disassemble, decompile, decipher, attack, emulate, exploit or reverse-engineer these portion of code;
 - iv. Apply it to any third-party products or services without Licensor’s permission;
 - v. Publish, copy, rent, lease, sell, export, import, distribute or lend any products containing these portions of code;
 - vi. Allow others to use any services relevant to the technology of these codes;
 - vii. Conduct any other act beyond the scope of this Licensing Agreement.
 g.  This Licensing Agreement terminates immediately if the Licensee breaches this Agreement. The Licensor may claim compensation from the Licensee where the Licensee’s breach causes any damage to the Licensor.
 h.  The laws of the People's Republic of China apply to this Licensing Agreement.
 i.  This Agreement is made in both Chinese and English, and the Chinese version shall prevail the event of conflict.
 ****************************************************************************/

#include "ProgramLib.h"
#include "../gfx/Program.h"
#include "gfx/DeviceGraphics.h"

#include <regex>
#include <string>
#include <sstream>
#include <iostream>

namespace {
    uint32_t _shdID = 0;

    std::string generateDefines(const cocos2d::ValueMap& defMap)
    {
        std::string ret;
        for (const auto& def : defMap)
        {
            if (def.second.asBool())
            {
                ret += "#define "  + def.first + "\n";
            }
        }
        return ret;
    }

    std::string replaceMacroNums(const std::string str, const cocos2d::ValueMap& defMap)
    {
        cocos2d::ValueMap cache;
        std::string tmp = str;
        for (const auto& def : defMap)
        {
            if (def.second.getType() == cocos2d::Value::Type::INTEGER || def.second.getType() == cocos2d::Value::Type::UNSIGNED)
            {
                cache.emplace(def.first, def.second);
            }
        }

        for (const auto& def : cache)
        {
            std::regex pattern(def.first);
            tmp = std::regex_replace(tmp, pattern, def.second.asString());
        }
        return tmp;
    }

    using RegexReplaceCallback = std::function<std::string(const std::match_results<std::string::const_iterator>&)>;

    static std::string regexReplaceString(const std::string& text, const std::string& pattern, const RegexReplaceCallback& replaceCallback)
    {
        std::string ret = text;
        std::regex re(pattern);

        std::string::const_iterator text_iter = text.cbegin();
        std::match_results<std::string::const_iterator> results;

        size_t start = 0;
        size_t offset = 0;
        while (std::regex_search(text_iter, text.end(), results, re))
        {
            offset = start + results.position();
            std::string replacement = replaceCallback(results);
            ret = ret.replace(offset, results.length(), replacement);
            start = offset + replacement.length();

            text_iter = results[0].second;
        }

        return ret;
    }

    std::string unrollLoops(const std::string& text)
    {
        auto func = [](const std::match_results<std::string::const_iterator>& results) -> std::string {
            assert(results.size() == 5);
            std::string snippet = results[4].str();
            std::string replacePatternStr = "\\{" + results[1].str() + "\\}";
            std::regex replacePattern(replacePatternStr);

            int32_t parsedBegin = atoi(results[2].str().c_str());
            int32_t parsedEnd = atoi(results[3].str().c_str());
            if (parsedBegin < 0 || parsedEnd < 0)
            {
                RENDERER_LOGE("Unroll For Loops Error: begin and end of range must be an int num.");
            }

            std::string unroll;
            char tmp[256] = {0};
            for (int32_t i = parsedBegin; i < parsedEnd; ++i)
            {
                snprintf(tmp, 256, "%d", i);
                std::string replaceFormat(tmp);
                unroll += std::regex_replace(snippet, replacePattern, replaceFormat);
            }
            return unroll;
        };

        return regexReplaceString(text, "#pragma for (\\w+) in range\\(\\s*(\\d+)\\s*,\\s*(\\d+)\\s*\\)([\\s\\S]+?)#pragma endFor", func);
    }
}

std::string test_unrollLoops(const std::string& text)
{
    return unrollLoops(text);
}

RENDERER_BEGIN

ProgramLib::ProgramLib(DeviceGraphics* device, std::vector<Template>& templates)
: _device(device)
{
    RENDERER_SAFE_RETAIN(_device);
    
    for (auto& templ : templates)
        define(templ.name, templ.vert, templ.frag, templ.defines);
}

ProgramLib::~ProgramLib()
{
    RENDERER_SAFE_RELEASE(_device);
    _device = nullptr;
}

void ProgramLib::define(const std::string& name, const std::string& vert, const std::string& frag, ValueVector& defines)
{
    auto iter = _templates.find(name);
    if (iter != _templates.end())
    {
        RENDERER_LOGW("Failed to define shader %s: already exists.", name.c_str());
        return;
    }

    uint32_t id = ++_shdID;

    // calculate option mask offset
    uint32_t offset = 0;
    for (auto& def : defines)
    {
        ValueMap& oneDefMap = def.asValueMap();
        uint32_t cnt = 1;

        //IDEA: currently we don't use min, max.
//        if (def.min != -1 && def.max != -1) {
//            cnt = (uint32_t)std::ceil((def.max - def.min) * 0.5);
//
//            def._map = std::bind(&Option::_map_1, def, std::placeholders::_1);
//        }
//        else
//        {
//            def._map = std::bind(&Option::_map_2, def, std::placeholders::_1);
//        }

        offset += cnt;

        oneDefMap["_offset"] = offset;
    }

    std::string newVert = _precision + vert;
    std::string newFrag = _precision + frag;

    // store it
    auto& templ = _templates[name];
    templ.id = id;
    templ.name = name;
    templ.vert = newVert;
    templ.frag = newFrag;
    templ.defines = defines;
}

uint32_t ProgramLib::getKey(const std::string& name, int32_t ekey)
{
    auto iter = _templates.find(name);
    assert(iter != _templates.end());

    auto& tmpl = iter->second;
    return ekey | tmpl.id;
}

Program* ProgramLib::getProgram(const std::string& name, const ValueMap& defines, int32_t definesKey)
{
    uint32_t key = getKey(name, definesKey);
    auto iter = _cache.find(key);
    if (iter != _cache.end()) {
        iter->second->retain();
        return iter->second;
    }

    Program* program = nullptr;
    // get template
    auto templIter = _templates.find(name);
    if (templIter != _templates.end())
    {
        const auto& tmpl = templIter->second;
        std::string customDef = generateDefines(defines) + "\n";
        std::string vert = replaceMacroNums(tmpl.vert, defines);
        vert = customDef + unrollLoops(vert);
        std::string frag = replaceMacroNums(tmpl.frag, defines);
        frag = customDef + unrollLoops(frag);

        program = new Program();
        program->init(_device, vert.c_str(), frag.c_str());
        program->link();
        _cache.emplace(key, program);
    }

    return program;
}

RENDERER_END
