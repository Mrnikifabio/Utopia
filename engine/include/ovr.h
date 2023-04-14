/**
 * @file		uvr.h
 * @brief	Self-contained helper class for interfacing OpenGL and OpenVR. Shortened version of Overvision's OvVR module.
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 */
#pragma once



 //////////////
 // #INCLUDE //
 //////////////

// GLM:
#include <glm/gtc/packing.hpp> 
#include "UObject.h"

////////////////
// CLASS OvVR //
////////////////

/**
 * @brief OpenGL-OpenVR interface.
 */

namespace utopia {


class OvVR : UObject
{
    //////////
public: //
    //////////	   

    struct Controller;

    /**
     * Eye enums
     */
    enum OvEye
    {
        EYE_LEFT = 0,
        EYE_RIGHT = 1,

        // Terminator:
        EYE_LAST
    };


    /**
    * Constructor
    */
    OvVR(const std::string& name);


    /**
     * Destructor
     */
    ~OvVR() = default;


    /**
     * @brief Init VR components.
     * @return TF
     */
    bool init();

    /**
     * Release VR components.
     * @return TF
     */
    bool free();
    


    /**
     * Get tracking system name.
     * @return tracking system name
     */
    std::string getTrackingSysName();


    /**
     * Print render models to the screen.
     */
    bool printRenderModels();


    /**
     * Get manufacturer name.
     * @return manufacturer system name
     */
    std::string getManufacturerName();


    /**
     * Get model number (name).
     * @return model number (name)
     */
    std::string getModelNumber();


    /**
     * Get HMD proper horizontal resolution in pixels.
     * @return HMD horizontal resolution in pixels
     */
    unsigned int getHmdIdealHorizRes();


    /**
     * Get HMD proper vertical resolution in pixels.
     * @return HMD vertical resolution in pixels
     */
    unsigned int getHmdIdealVertRes();


    /**
     * Update poses and internal params. Invoke that once per frame.
     * @return TF
     */
    bool update();

    /**
     * Get the projection matrix for the given eye and plane params.
     * @param eye left or right eye (use enum)
     * @param nearPlane user camera near plane distance
     * @param farPlane user camera far plane distance
     * @return projection matrix ready for OpenGL
     */
    glm::mat4 getProjMatrix(OvEye eye, float nearPlane, float farPlane);


    /**
     * Get the eye-to-head modelview matrix for the given eye.
     * @param eye left or right eye (use enum)
     * @return eye-to-head modelview matrix ready for OpenGL
     */
    glm::mat4 getEye2HeadMatrix(OvEye eye);

    /**
     * Get the user's head modelview position.
     * @return modelview matrix ready for OpenGL
     */
    glm::mat4 getModelviewMatrix();


    /**
     * Get the number of identified and initialized controllers.
     * @return number of controllers
     */
    unsigned int getNrOfControllers();

    /**
     * Get pointer to internal controller reference.
     * @param pos controller position in the list
     * @return pointer or nullptr if error
     */
    Controller* getController(unsigned int pos) const;

    /**
     * Enable/disable reprojection
     * @param flag true or false
     */
    void setReprojection(bool flag);


    /**
     * Pass the left and right textures to the HMD.
     * @param eye left or right eye (use enum)
     * @param eyeTexture OpenGL texture handle
     */
    void pass(OvEye eye, unsigned int eyeTexture);


    /**
     * Once passed the left and right textures, invoke this method to terminate rendering.
     */
    void render() override;


    // Controller data:
    struct Controller;

    private:
        struct pimpl;

        std::unique_ptr<pimpl> m_pimpl;
};

}
