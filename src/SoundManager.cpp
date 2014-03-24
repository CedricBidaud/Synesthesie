#include "SoundManager.hpp"

#include <iostream>
#include <string.h>

#include <FMOD/fmod.h>

namespace imac3{

SoundManager::SoundManager(){
	minVolume = 1000;
	maxVolume = 0;
	calibrationDuration = 0;
	//~ m_band = band;
}

void SoundManager::PrintInterfaces(){
	// DEBUG : voir les interfaces
	char nomMicro[100] = {0};
	for(int i = 0; i < input; i++)
	{
		FMOD_System_GetRecordDriverInfo(system, i, nomMicro, 100, 0);
		//printf("%d - %s\n", i, nomMicro);
	}
	char nomHP[100] = {0};
	for(int i = 0; i < output; i++)
	{
		FMOD_System_GetDriverInfo(system, i, nomHP, 100, 0);
		//printf("%d - %s\n", i, nomHP);
	}
}


void SoundManager::Init(int size){

    //int input, output;
 
    //FMOD_SYSTEM *system;
    
    spectrumSize=size;
 
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 2, FMOD_INIT_NORMAL, 0);
    FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_DSOUND);
 
    FMOD_SOUND *sound;
    FMOD_System_CreateSound(system, 0, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENUSER, 0, &sound);
    
    
 
	// Ouput FMOD
 
    //FMOD_CHANNEL *channel = 0;
    channel=0;
 
	// Input FMOD
 
    FMOD_CREATESOUNDEXINFO exinfo;
    FMOD_System_GetRecordNumDrivers(system, &input);
 
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
 
    exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels      = 1;
    exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
    exinfo.defaultfrequency = 44100;
    exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels / 10;
    // Enregistrera pendant 10ms, si tu veux 100ms par exemple, enlève le "/ 10"
 
    FMOD_System_CreateSound(system, 0, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound);
    
    //spectrum[4096] = {0};
    //std::cout<<"Spectrum should be created. Size : "<<spectrum[200]<<std::endl;
    
	// Définit les haut-parleurs
	FMOD_System_SetDriver(system, output);
	// Initialise l'envoi du son récupéré du micro dans les haut-parleurs
	FMOD_Sound_SetMode(sound, FMOD_LOOP_NORMAL);
	 
	// Commence la récupération du son
	input=0; // définition de l'input (0 = default)
	FMOD_System_RecordStart(system, input, sound, 1);
	// Commence l'envoi du son récupéré du micro dans les haut-parleurs
	FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, 0, &channel);
	FMOD_Channel_SetVolume(channel, 0);
	
}

void SoundManager::GetSpectrum(float* spectrum){
	FMOD_Channel_GetSpectrum(channel, spectrum, spectrumSize, 0, FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS);	
}

float SoundManager::GetMaxFrequency(float* spectrum, int lowBound, int highBound){
	
	lowBound=ToIndex(lowBound);
	if (lowBound<0) {lowBound=0;}
	highBound=ToIndex(highBound);
	if (highBound>spectrumSize) {highBound=spectrumSize;}
	
	float maxFreq=spectrum[lowBound];
	int maxI=lowBound;
	
	for (int i=lowBound; i<highBound; i++){
		if (spectrum[i] > maxFreq){
			maxFreq=spectrum[i];
			maxI=i;
		} 
	}
	
	return ToFrequency(maxI);	
}

float SoundManager::GetMaxFrequency(float* spectrum){
	return GetMaxFrequency(spectrum, 0, 22050);	
}

float SoundManager::GetRelMaxFrequency(float* spectrum, int lowBound, int highBound){
	
	lowBound=ToIndex(lowBound);
	if (lowBound<0) {lowBound=0;}
	highBound=ToIndex(highBound);
	if (highBound>spectrumSize) {highBound=spectrumSize;}
	
	float maxFreq=spectrum[lowBound];
	int maxI=lowBound;
	
	for (int i=lowBound; i<highBound; i++){
		if (spectrum[i] > maxFreq){
			maxFreq=spectrum[i];
			maxI=i;
			
		} 
	}
	
	float res=((float)maxI-lowBound)/((float)highBound-lowBound);
	return res;	
}

float SoundManager::GetRelMaxFrequency(float* spectrum){
	return GetRelMaxFrequency(spectrum, 0, 22050);
}

float SoundManager::GetVolume(float* spectrum, int lowBound, int highBound){
	
	lowBound=ToIndex(lowBound);
	if (lowBound<0) {lowBound=0;}
	highBound=ToIndex(highBound);
	if (highBound>spectrumSize) {highBound=spectrumSize;}
	
	float total=0;
	
	for (int i=lowBound; i<highBound; i++){
		total+=spectrum[i];
	}
	
	return total;	
}

float SoundManager::GetVolume(float* spectrum){
	return GetVolume(spectrum, 0, spectrumSize);
}

int SoundManager::ToIndex(int frequency){
	return (int)frequency*spectrumSize/22050;
}

int SoundManager::ToFrequency(int index){
	return (int)index*spectrumSize/4096;
}

void SoundManager::Update(){
	FMOD_System_Update(system);
}

void SoundManager::initCalibration(){
	maxVolume = 0;
	minVolume = 1000;
}

void SoundManager::calibrateVolume(float * spectrum){
	float vol = GetVolume(spectrum);
	if(vol > maxVolume) maxVolume = vol;
	if(vol < minVolume) minVolume = vol;
	
	--calibrationDuration;
	if(calibrationDuration == 0){
		std::cout << "Calibration terminée" << std::endl;
		std::cout << "Max volume : " << maxVolume << std::endl;
		std::cout << "Min volume : " << minVolume << std::endl;
	}
}

int SoundManager::getCalibrationDuration(){
	return calibrationDuration;
}

void SoundManager::setCalibrationDuration(int duration){
	calibrationDuration = duration;
}

float SoundManager::getMaxVolume(){
	return maxVolume;
}

float SoundManager::getMinVolume(){
	return minVolume;
}

float SoundManager::scaleVolume(float volume){
	if(maxVolume != 0){
		return((volume - minVolume) / maxVolume);
	}else{
		return -1;
	}
}

void SoundManager::setMinMax(float min, float max){
	minVolume = min;
	maxVolume = max;
}

SoundManager::~SoundManager(){
	
}

} // end namespace imac3
