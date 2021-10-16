#ifndef mobility_h // Start of precompiler check to avoid dupicate inclusion of this code block.

#define mobility_h // Precompiler macro used for precompiler check.

bool mobilityStatus = false; 

/**
 * @brief Initialize robot drive train.
 * @details Robot moves on two DC motors driven via an I2C motor controller.
 * ==========================================================================*/
void initMobility()
{
   Log.traceln("<initMobility> Initialize the drive train for this platform.");
} // initMobiity. 

#endif // End of precompiler protected code block