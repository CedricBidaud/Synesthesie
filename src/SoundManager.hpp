#ifndef _IMAC3_SOUNDMANAGER_HPP
#define _IMAC3_SOUNDMANAGER_HPP

#include <FMOD/fmod.h>

namespace imac3 {

class SoundManager{
	public:
		SoundManager();
		void PrintInterfaces();
		void Init(int);
		void GetSpectrum(float*);
		float GetMaxFrequency(float*, int, int); // Get the value of the maximal frequency between to bounds (Hz)
		float GetRelMaxFrequency(float*, int, int); // Get the relative position (between 0 and 1) of the highest value frequency between a min and max bounds
		float GetMaxFrequency(float*); // Default values : 0Hz > 22050Hz
		float GetRelMaxFrequency(float*); // Default values : 0Hz > 22050Hz
		float GetVolume(float*, int, int);
		float GetVolume(float*);
		
		void Update();
		
		int ToIndex(int); // Translate a sound frequency into the corresponding spectrum index
		int ToFrequency(int); // Translate a spectrum index into the corresponding frequency
		
		void calibrateVolume(float * spectrum);
		int getCalibrationDuration();
		void setCalibrationDuration(int duration);
		float scaleVolume(float volume);
		
		~SoundManager();
		
		
	private:
		FMOD_SYSTEM *system;
		FMOD_CHANNEL *channel;
		int input, output;
		int spectrumSize;
		float minVolume = 1000, maxVolume = 0;
		int calibrationDuration = 0;
		
};

}

#endif
