# Utopia

Utopia is a graphics engine developed as the final project for the Computer Graphics course and further enhanced during the Virtual Reality course at the University of Applied Sciences of Southern Switzerland.

The engine is built using the following libraries and technologies:
- OpenGL graphics library
- GLM mathematics library
- FreeGLUT for window management
- GLEW for OpenGL extension loading
- FreeImage for image loading

Special emphasis has been placed on ensuring compatibility with the OpenVR framework and the LeapMotion SDK. This enables Utopia to work seamlessly with compatible Virtual Reality headsets and the LeapMotion hands tracking device.

## Platform Compatibility

- Utopia works exclusively on Windows in its latest release, as OpenVR is designed specifically for this platform.
- The initial version, based on the older OpenGL 1.x API, is compatible with Linux, although it hasn't undergone extensive testing on that platform.

## TowerCraneSimulator Example

Utopia includes a sample application called TowerCraneSimulator, which serves as a proof-of-concept for the engine's capabilities. The TowerCraneSimulator example leverages Utopia's features, including Virtual Reality headset integration and LeapMotion hand tracking.

### Controls

The following controls are available in the TowerCraneSimulator example:

#### Keyboard Controls

- **Camera Movement:**
  - 'a' or 'A': Move the camera to the left.
  - 'd' or 'D': Move the camera to the right.
  - 's' or 'S': Move the camera backward.
  - 'w': Move the camera forward.
  - 'r': Move the camera upward.
  - 'f': Move the camera downward.

- **Display Modes:**
  - 'z': Enable solid mode.
  - 'x': Enable wireframe mode.

- **Anisotropy and Texture Visualization:**
  - 'c': Cycle through different levels of anisotropy.
  - 'v': Cycle through different textures visualization modes.

- **Camera Switching:**
  - '1': Switch to the free camera.
  - '2': Switch to the tower camera.
  - '3': Switch to the fixed camera.

- **Box Interaction:**
  - 'e': Hook or release a box (if available and not already hooking).

- **Fisical Hook Movement:**
  - '-': Move the fisical hook of the tower downwards.
  - '+': Move the fisical hook of the tower upwards.

- **Tower Camera Node Movement:**
  - 'n': Move the tower camera node to the left.
  - 'u': Move the tower camera node upward.
  - 'j': Move the tower camera node downward.
  - 'm': Move the tower camera node to the right.
  - 'h': Move the tower camera node backward.
  - 'k': Move the tower camera node forward.

#### Special Controls

- **Tower Rotation:**
  - Left arrow key: Rotate the tower counterclockwise.
  - Right arrow key: Rotate the tower clockwise.

- **Hook Movement:**
  - Up arrow key: Move the hook of the tower forward.
  - Down arrow key: Move the hook of the tower backward.

### To test the TowerCraneSimulator example with an OpenVR compatible headset:
1. Navigate to the "conf" folder.
2. Open the global.conf file.
3. Set the value of the "openvr" variable to true.
4. Restart the application

## Documentation

For a detailed explanation of Utopia's implementation, please refer to the accompanying PDF document located in the doc folder.
