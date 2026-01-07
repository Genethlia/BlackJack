#include "settings.h"

Settings::Settings(){
	LoadSettings();
}

void Settings::Draw(){
	dealerHitOn17.Draw();
	musicOn.Draw();
	soundEffectsOn.Draw();
}

void Settings::Update(){
	dealerHitOn17.Update();
	musicOn.Update();
	soundEffectsOn.Update();
}

void Settings::SaveSettings(){
	ofstream fout("saves/settings.txt");
	if (fout.is_open()) {
		fout << "DealerHitOn17 " << ShouldDealerHitOn17 << endl;
		fout << "Music? " << IsMusicOn << endl;
		fout << "SoundEffects? " << IsSoundEffectsOn << endl;
	}
}

void Settings::LoadSettings(){
	string label;
	ifstream fin("saves/settings.txt");
	if (fin.is_open()) {
		fin >> label >> ShouldDealerHitOn17;
		fin >> label >> IsMusicOn;
		fin >> label >> IsSoundEffectsOn;
	}
	else {
		ShouldDealerHitOn17 = false;
		IsMusicOn = false;
		IsSoundEffectsOn = true;
	}
}

void Settings::SetTransform(float* scale, float* offsetX, float* offsetY){
	dealerHitOn17.SetTransform(scale, offsetX, offsetY);
	musicOn.SetTransform(scale, offsetX, offsetY);
	soundEffectsOn.SetTransform(scale, offsetX, offsetY);
}
