/*!
@cond GEN_REFERENCE_BOOK

@mainpage Cocos Runtime Native Renderer

@image html img-cocos.jpg

@section sec1 Introduction
Native Renderer is an abstraction of game engine's rendering process, it's designed to achieve maximum performance based on JavaScript Bounding architecture.

JavaScript Bounding is the fundamental technology which permit a web game running on runtime like environment, it provides JavaScript virtual machine and port WebGL APIs to OpenGL APIs or other rendering backend APIs.

However, when the bounding level is built upon WebGL APIs, game logic and game engine is still running in JavaScript which has a huge limitation on performance and native features like multi thread or SIMD operations.

The Native Renderer is exactly designed to solve these problems, by raise up the bounding level to an abstract and general purpose native render engine.

To better explain the architecture, the upper part of the graph below is showing how we build the relationship between JavaScript engine and Native Renderer. Then a sequence graph shows how the general rendering process looks like.

@image html native-renderer-sequence.png

@section sec2 License
Xiamen Yaji Software Co., Ltd., (the “Licensor”) grants the user (the “Licensee”) non-exclusive\n
and non-transferable rights to use the software according to the following conditions:\n
- a.  The Licensee shall pay royalties to the Licensor, and the amount of those royalties and the\n
    payment method are subject to separate negotiations between the parties.\n
- b.  The software is licensed for use rather than sold, and the Licensor reserves all rights over\n
    the software that are not expressly granted (whether by implication, reservation or \n
    prohibition).\n
- c.  The open source codes contained in the software are subject to the MIT Open Source Licensing\n
    Agreement (see the attached for the details);\n
- d.  The Licensee acknowledges and consents to the possibility that errors may occur during the\n
    operation of the software for one or more technical reasons, and the Licensee shall take\n
    precautions and prepare remedies for such events. In such circumstance, the Licensor shall\n
    provide software patches or updates according to the agreement between the two parties.\n
    the Licensor will not assume any liability beyond the explicit wording of this Licensing\n
    Agreement.\n
- e.  Where the Licensor must assume liability for the software according to relevant laws, the\n
    Licensor’s entire liability is limited to the annual royalty payable by the Licensee.\n
- f.  The Licensor owns the portions listed in the root directory and subdirectory (if any) in\n
    the software and enjoys the intellectual property rights over those portions. As for the\n
    portions owned by the Licensor, the Licensee shall not:\n
    + i. Bypass or avoid any relevant technical protection measures in the products or services;\n
    + ii. Release the source codes to any other parties;\n
    + iii. Disassemble, decompile, decipher, attack, emulate, exploit or reverse-engineer these\n
        portion of code;\n
    + iv. Apply it to any third-party products or services without Licensor’s permission;\n
    + v. Publish, copy, rent, lease, sell, export, import, distribute or lend any products\n
        containing these portions of code;\n
    + vi. Allow others to use any services relevant to the technology of these codes;\n
    + vii. Conduct any other act beyond the scope of this Licensing Agreement.\n
- g.  This Licensing Agreement terminates immediately if the Licensee breaches this Agreement.\n
    The Licensor may claim compensation from the Licensee where the Licensee’s breach causes\n
    any damage to the Licensor.\n
- h.  The laws of the People's Republic of China apply to this Licensing Agreement.\n
- i.  This Agreement is made in both Chinese and English, and the Chinese version shall prevail\n
    the event of conflict.\n


附件：MIT开源协议\n
Attached: MIT Open Source Agreement\n
著作权（2017-2018）：厦门雅基\n
Copyright (c) 2017-2018 Xiamen Yaji\n
著作权（2013-2016）：触控科技\n
Copyright (c) 2013-2016 Chukong Technologies\n
著作权（2010-2012）：cocos2d-x 社区\n
Copyright (c) 2010-2012 cocos2d-x community\n
\n
特此免费授予获得这个软件和相关文档文件（“软件”）的任何人在满足以下条件的情况下，\n
不受限制地处理该软件，包括但不限于使用、复制、修改、合并、发布、分发、再许可和/或\n
出售本软件的副本的权利，以及允许被提供给软件的人这么做：\n
Permission is hereby granted, free of charge, to any person obtaining a copy of this\n
software and associated documentation files (the "Software"), to deal in the Software\n
without restriction, including without limitation the rights to use, copy, modify, merge,\n
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons\n
to whom the Software is furnished to do so, subject to the following conditions:\n
须将上述版权声明及该许可声明置于本软件的所有副本或重要部分中。\n
The above copyright notice and this permission notice shall be included in all copies or\n
substantial portions of the Software.\n
本软件由版权持有人及其他责任者“按原样”提供，不做任何明示或暗示的保证（包括但不限于\n
其适销性和特殊目的的适用性及不侵权的保证）。在任何情况下，不论是在合同之诉、侵权之\n
诉还是其它，本软件或使用本软件或处理软件造成的或关联的任何索赔、赔偿或其它责任，版\n
权持有人或作者概不承担任何责任。\n
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, \n
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR\n
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE\n
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR\n
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER\n
DEALINGS IN THE SOFTWARE.\n
\n

@endcond
*/
