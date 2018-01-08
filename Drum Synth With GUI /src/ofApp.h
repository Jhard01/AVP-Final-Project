#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    //Audio Output Method
    void audioOut(float * output, int bufferSize, int nchannels);
    
    //SoundStream Object & Setup variables
    unsigned int bufferSize, sampleRate;
    ofSoundStream soundSt;
    
    //Audio Variables & Maximillian Objects
    
    // Kick //
    maxiOsc kickOsc;
    maxiFilter kickFilt;
    maxiEnv kickEnvelope;
    double kickEnv;
    int kickTrigger;
    double kickOut[2];
    
    // Snare //
    maxiOsc snareOsc[3];
    maxiFilter snareFilt[3];
    maxiEnv snareEnvelope;
    double snareEnv;
    int snareTrigger;
    double snareOut[3];
    
    // Closed Hat //
    maxiOsc cHatOsc[3];
    maxiFilter cHatFilt[3];
    maxiEnv cHatEnvelope;
    double cHatEnv;
    int cHatTrigger;
    double cHatOut[3];
    
    // Open Hat //
    maxiOsc oHatOsc[3];
    maxiFilter oHatFilt[3];
    maxiEnv oHatEnvelope;
    double oHatEnv;
    int oHatTrigger;
    double oHatOut[3];
    
    // Cowbell //
    maxiOsc cowOsc[6];
    maxiFilter cowFilt[2];
    maxiEnv cowEnvelope;
    double cowEnv;
    int cowTrigger;
    double cowOut[3];
    
    // Low Tom //
    maxiOsc lowTomOsc[5];
    maxiFilter lowTomFilt[3];
    maxiEnv lowTomEnvelope;
    double lowTomEnv;
    int lowTomTrigger;
    double lowTomOut[4];
    
    // Bell //
    maxiOsc bellOsc[6];
    maxiFilter bellFilt[2];
    maxiEnv bellAmpEnvelope, bellPitchEnvelope;
    double bellAmpEnv, bellPitchEnv;
    int bellTrigger, bellCount;
    double bellOut[4];
    
    //Sequencer Controls
    maxiOsc timer;
    maxiOsc swingLfo;
    double swingLfoOut;
    int current, last, playHead, barNumber;
    int sequence[7][16];
    int posX, posY;
    bool changeBeat, lastPress, changeRhythm, lastRhythm;
    
    //GUI Setup
    ofxPanel timerGui;
    ofxFloatSlider speed;
    ofxToggle swing;
    ofxToggle metroOn;
    ofxFloatSlider globalpitch; 
    
    // Kick //
    int kickSeq[16]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    // Snare //
    int snareSeq[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    // Closed Hat //
    int cHatSeq[16]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    // Open Hat //
    int oHatSeq[16]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    // Cowbell //
    int cowSeq[16]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    // Low Tom //
    int lowTomSeq[16]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    // Bell    //
    int bellSeq[16]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    // Inital Sequence //
    int empty[16]    = {0, 0, 0, 0, 0, 0 ,0, 0, 0, 0 ,0, 0, 0, 0 ,0, 0};
    
    
};

