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

#include <stdint.h>
#include "base/CCVector.h"
#include "base/ccCArray.h"
#include "../Macro.h"

RENDERER_BEGIN

class Camera;
class Light;
class Model;
class View;

/**
 * @addtogroup renderer
 * @{
 */

/**
 *  @brief Render scene.
 */
class Scene
{
public:
    /**
     *  @brief The default constructor.
     */
    Scene();
    
    /**
     *  @brief Resets all model culling mask.
     */
    void reset();
    /**
     *  @brief Sets debug camera.
     *  @param[in] debugCamera Debug camera pointer.
     */
    void setDebugCamera(Camera* debugCamera);
    
    /**
     *  @brief Gets cameras count.
     *  @return Cameras count.
     */
    inline uint32_t getCameraCount() const { return (uint32_t)_cameras.size(); }
    
    /**
     *  @brief Gets camera by index.
     *  @return camera pointer.
     */
    Camera* getCamera(uint32_t index) const;
    /**
     *  @brief Adds camera.
     *  @param[in] camera Camera pointer.
     */
    void addCamera(Camera* camera);
    /**
     *  @brief Removes camera.
     *  @param[in] camera Camera pointer.
     */
    void removeCamera(Camera* camera);
    /**
     *  @brief Gets all cameras.
     *  @return All Cameras container.
     */
    inline const Vector<Camera*>& getCameras() const { return _cameras; }
    /**
     *  @brief Sorts all cameras.
     */
    void sortCameras();
    
    /**
     *  @brief Gets all models count.
     *  @return All models count.
     */
    inline uint32_t getModelCount() const { return (uint32_t)_models.size(); }
    /**
     *  @brief Gets model by index.
     *  @param[in] index Model index.
     *  @return Model pointer.
     */
    Model* getModel(uint32_t index);
    /**
     *  @brief Adds model.
     *  @param[in] model Model pointer.
     */
    void addModel(Model* model);
    /**
     *  @brief Removes model.
     *  @param[in] model Model pointer.
     */
    void removeModel(Model* model);
    /**
     *  @brief Removes all models.
     */
    void removeModels();
    /**
     *  @brief Gets all models.
     *  @return Models container.
     */
    inline const std::vector<Model*>& getModels() const { return _models; }
    
    /**
     *  @brief Gets light count.
     *  @return Lights count.
     */
    inline uint32_t getLightCount() const { return (uint32_t)_lights.size(); }
    /**
     *  @brief Gets light by index.
     *  @param[in] index Light index.
     *  @return Light pointer.
     */
    Light* getLight(uint32_t index);
    /**
     *  @brief Adds light.
     *  @param[in] light Light pointer.
     */
    void addLight(Light* light);
    /**
     *  @brief Removes light.
     *  @param[in] light Light pointer.
     */
    void removeLight(Light* light);
    
    /**
     *  @brief Adds view.
     *  @param[in] view View pointer.
     */
    void addView(View* view);
    /**
     *  @brief Removes view.
     *  @param[in] view View pointer.
     */
    void removeView(View* view);
    
private:
    //REFINE: optimize speed.
    Vector<Camera*> _cameras;
    Vector<Light*> _lights;
    std::vector<Model*> _models;
    Vector<View*> _views;
    Camera* _debugCamera = nullptr;
};

// end of renderer group
/// @}

RENDERER_END
