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

#pragma once

#include <vector>
#include <list>
#include "base/CCVector.h"
#include "base/CCValue.h"
#include "base/ccCArray.h"
#include "math/Mat4.h"
#include "../Macro.h"

RENDERER_BEGIN

class Effect;
class InputAssembler;
class Model;

/**
 * @addtogroup renderer
 * @{
 */

struct DrawItem
{
    Model* model = nullptr;
    InputAssembler* ia = nullptr;
    Effect* effect = nullptr;
    ValueMap* defines = nullptr;
};

class Model;

class ModelPool
{
public:
    static Model* getOrCreateModel();
    static void returnModel(Model*);
    
private:
    static ccCArray* _pool;;
};

/**
 *  @brief Model contains InputAssembler, effect, culling mask and model matrix.
 */
class Model
{
public:
    /**
     *  @brief The default constructor.
     */
    Model();
    /**
     *  @brief The default destructor.
     */
    ~Model();
    /**
     *  @brief Gets input assembler count.
     */
    inline uint32_t getInputAssemblerCount() const { return (uint32_t)_inputAssemblers.size(); }
    /**
     *  @brief Indicates whether the model's data is in dynamic input assembler.
     */
    inline bool isDynamicIA() const { return _dynamicIA; }
    /**
     *  @brief Sets whether the model's data is in dynamic input assembler.
     */
    inline void setDynamicIA(bool value) { _dynamicIA =  value; }
    /**
     *  @brief Gets draw item count.
     */
    inline uint32_t getDrawItemCount() const { return _dynamicIA ? 1 :  (uint32_t)_inputAssemblers.size(); }
    /**
     *  @brief Sets model matrix.
     */
    inline void setWorldMatix(const Mat4& matrix) { _worldMatrix = std::move(matrix); }
    /**
     *  @brief Gets mode matrix.
     */
    inline const Mat4& getWorldMatrix() const { return _worldMatrix; }
    /**
     *  @brief Sets culling mask.
     */
    inline void setCullingMask(int val) { _cullingMask = val; }
    /**
     *  @brief Gets culling mask.
     */
    inline int getCullingMask() const { return _cullingMask; }
    /**
     *  @brief Adds a input assembler.
     */
    void addInputAssembler(const InputAssembler& ia);
    /**
     *  @brief Clears all input assemblers.
     */
    void clearInputAssemblers();
    /**
     *  @brief Adds an effect.
     */
    void addEffect(Effect* effect);
    /**
     *  @brief Clears all effects.
     */
    void clearEffects();
    /**
     *  @brief Extract draw item for the given index during rendering process.
     */
    void extractDrawItem(DrawItem& out, uint32_t index) const;

private:
    friend class ModelPool;
    void reset();
    
    Mat4 _worldMatrix;
    ccCArray* _effects = ccCArrayNew(2);
    
    std::vector<InputAssembler> _inputAssemblers;
    std::vector<ValueMap*> _defines;
    bool _dynamicIA = false;
    int _cullingMask = -1;
};

// end of renderer group
/// @}

RENDERER_END
