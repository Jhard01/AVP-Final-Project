#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //Audio Settings
    sampleRate = 44100;
    bufferSize = 512;
    
    //OF Setup
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    //GUI Setup
    timerGui.setup();
    timerGui.add(speed.setup("Timer Speed", 5,1,10));
    timerGui.add(swing.setup("Swing", false));
    timerGui.add(metroOn.setup("Timer On",true));
     timerGui.add(globalpitch.setup("Global Pitch",0,-50,50));
    
    
    //Sequencer Setup
    barNumber = 1;
    
    //Assigning the sequencer patterns
    for (int i=0; i<16; i++){
        sequence[0][i] = kickSeq[i];
        sequence[1][i] = snareSeq[i];
        sequence[2][i] = cHatSeq[i];
        sequence[3][i] = oHatSeq[i];
        sequence[4][i] = cowSeq[i];
        sequence[5][i] = lowTomSeq[i];
        sequence[6][i] = bellSeq[i];
    }
    
    //Setting all the envelope times
    // Kick //
    kickEnvelope.setAttack(1);
    kickEnvelope.setDecay(185);
    kickEnvelope.setSustain(1);
    kickEnvelope.setRelease(400);
    
    // Snare //
    snareEnvelope.setAttack(5);
    snareEnvelope.setDecay(250);
    snareEnvelope.setSustain(1);
    snareEnvelope.setRelease(250);
    
    // Closed Hat //
    cHatEnvelope.setAttack(1);
    cHatEnvelope.setDecay(5);
    cHatEnvelope.setSustain(0.2);
    cHatEnvelope.setRelease(100);
    
    // Open Hat //
    oHatEnvelope.setAttack(50);
    oHatEnvelope.setDecay(100);
    oHatEnvelope.setSustain(0.2);
    oHatEnvelope.setRelease(200);
    
    // Cowbell //
    cowEnvelope.setAttack(5);
    cowEnvelope.setDecay(50);
    cowEnvelope.setSustain(1);
    cowEnvelope.setRelease(350);
    
    // Low Tom //
    lowTomEnvelope.setAttack(1);
    lowTomEnvelope.setDecay(10);
    lowTomEnvelope.setSustain(1);
    lowTomEnvelope.setRelease(300);
    
    // Bell //
    bellAmpEnvelope.setAttack(1);
    bellAmpEnvelope.setDecay(10);
    bellAmpEnvelope.setSustain(1);
    bellAmpEnvelope.setRelease(500);
    
    bellPitchEnvelope.setAttack(1);
    bellPitchEnvelope.setDecay(200 );
    bellPitchEnvelope.setSustain(0.2);
    bellPitchEnvelope.setRelease(100);
    
    //SoundStream
    soundSt.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //Mapping the mouseX positions to the step sequencer divisions of the screen
    posX = ofMap(mouseX, 0, ofGetWidth() - ((ofGetWidth()/16 - 20)), 0, 15);
    
    if(mouseY > (ofGetHeight()/2 -50)){
        posY = ofMap(mouseY, (ofGetHeight()/2) - 50, ofGetHeight(), 0 ,8 );
    }
    
    //Changing the beat
    if(changeBeat == true && lastPress == false){
        sequence[posY][posX] = !sequence[posY][posX];
    }
    for(int i=0; i < 16; i++){
        if (changeRhythm && !lastRhythm){
            sequence[posY][empty[i]];
        }
        kickSeq[i]   = sequence[0][i];
        snareSeq[i]  = sequence[1][i];
        cHatSeq[i]   = sequence[2][i];
        oHatSeq[i]   = sequence[3][i];
        cowSeq[i]    = sequence[4][i];
        lowTomSeq[i] = sequence[5][i];
        bellSeq[i]   = sequence[6][i];

    }
    lastPress = changeBeat;
    lastRhythm = changeRhythm;
    
    if(metroOn){
    if(swing)
    {
        swingLfoOut = speed + (swingLfo.sinebuf(speed*200)*2.5);
    } else {
        swingLfoOut = speed;
    }
    }else{
        swingLfoOut = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //Calculate the size of each step
    int step = ofGetWidth() / 16;
    
    //Drawing the steps
    for(int i=0; i <=16; i++){
        for(int j=0; j < 7; j++){
            if (sequence[j][i] == 1){
                ofSetColor(250, 250, 250, 100); // if selected
            }else{
                ofSetColor(50);
            }
            ofRect( i * step + 20, ((ofGetHeight()/2) - 50) +(j * 50), step - 20, 30);
            ofSetColor(100, 100, 10, 5);
            ofRect( (posX * step)  + 10 , ((ofGetHeight()/2) - 50) + (posY * 50) - 10, step , step);
            
            // drawing the metronome square
            if(playHead%16 == i){
                ofSetColor(150);
            }
            ofCircle((i * ofGetWidth()/16) + (ofGetWidth()/16/2)+10,((ofGetHeight()/2) - 80), 10);
            
        }
    }
    timerGui.draw();
}

//--------------------------------------------------------------
void ofApp::audioOut(float *output, int bufferSize, int nChannels){
    
    for(int i=0; i < bufferSize; i++){
        //Envelope Triggers
        kickEnv  = kickEnvelope.adsr(1., kickEnvelope.trigger);
        snareEnv = snareEnvelope.adsr(1., snareEnvelope.trigger);
        cHatEnv = cHatEnvelope.adsr(1., cHatEnvelope.trigger);
        oHatEnv = oHatEnvelope.adsr(1., oHatEnvelope.trigger);
        cowEnv  = cowEnvelope.adsr(1., cowEnvelope.trigger);
        lowTomEnv = lowTomEnvelope.adsr(1. , lowTomEnvelope.trigger);
        bellAmpEnv = bellAmpEnvelope.adsr(1., bellAmpEnvelope.trigger);
        bellPitchEnv = bellPitchEnvelope.adsr(1., bellPitchEnvelope.trigger);
        
        //Counter Speed //
        current = (int)timer.phasor(swingLfoOut);
        
        if(last != current){
            kickTrigger = kickSeq[playHead%16];
            snareTrigger = snareSeq[playHead%16];
            cHatTrigger = cHatSeq[playHead%16];
            oHatTrigger = oHatSeq[playHead%16];
            cowTrigger = cowSeq[playHead%16];
            lowTomTrigger = lowTomSeq[playHead%16];
            bellTrigger = bellSeq[playHead%16];
            playHead++;
            last = 0;
            
            if(playHead%16 == 0){
                barNumber++;
            }
        }
        
        if(kickTrigger == 1 ) {
            kickEnvelope.trigger = 1;
        }else{
            kickEnvelope.trigger = 0;
        }
        if(snareTrigger == 1){
            snareEnvelope.trigger = 1;
        }else{
            snareEnvelope.trigger = 0;
        }
        if(cHatTrigger == 1){
            cHatEnvelope.trigger = 1;
        }else{
            cHatEnvelope.trigger = 0;
        }
        if(oHatTrigger == 1){
            oHatEnvelope.trigger = 1;
        }else{
            oHatEnvelope.trigger = 0;
        }
        if(cowTrigger == 1){
            cowEnvelope.trigger = 1;
        }else{
            cowEnvelope.trigger = 0;
        }
        if(lowTomTrigger == 1){
            lowTomEnvelope.trigger = 1;
        }else{
            lowTomEnvelope.trigger = 0;
        }
        if (bellTrigger == 1)
        {
            bellCount ++;
            if (bellCount %2 ==0){
                bellAmpEnvelope.trigger = 1;
                bellPitchEnvelope.trigger = 1;
            }
        } else {
            bellAmpEnvelope.trigger = 0;
            bellPitchEnvelope.trigger = 0;
        }
        
        float fMod = (lowTomEnv + kickEnv / 2) + 0.5;
        
        kickOut[0] = kickOsc.sinewave((200 * kickEnv) * fMod+globalpitch) ;
        kickOut[1] = kickEnv * (kickFilt.lores(kickOut[0], 200, 7));
        
        snareOut[0] = snareFilt[0].lores(snareOsc[0].noise(), 7500, 5) / 3;
        snareOut[1] = snareFilt[1].lores(snareOsc[1].sinewave(200+globalpitch) + snareOut[0], 2500, 10) ;
        snareOut[2] = snareEnv * ((snareFilt[2].lores(snareOut[1], 5000, 2) ) );
        
        cHatOut[0] = cHatOsc[0].noise() + cHatOsc[1].sinewave(3000 + (1111*cHatOsc[2].sinewave(500))+globalpitch);
        cHatOut[1] = cHatFilt[0].hires(cHatOut[0], 400, 1.5);
        cHatOut[2] = (cHatOut[1] * cHatEnv);
        
        oHatOut[0] = oHatOsc[0].noise() + oHatOsc[1].sinewave(3000 + (1111*oHatOsc[2].sinewave(500))+globalpitch);
        oHatOut[1] = oHatFilt[0].hires(oHatOut[0], 400, 1.5);
        oHatOut[2] = (oHatOut[1] * oHatEnv) ;
        
        lowTomOut[0] = lowTomOsc[0].sinewave(320 * fMod + (50*lowTomOsc[1].sinewave(200))+globalpitch);
        lowTomOut[2] = lowTomOut[0] * lowTomEnv ;
        
        bellOut[0] = (bellPitchEnv + 1) /2 ;
        bellOut[1] =  ((bellOsc[0].noise() * bellOut[0])  + bellOsc[1].sinewave(100.412 * ( bellOsc[2].sinewave(501.003415+globalpitch) *  (bellOut[0] * 18.12435) ))+globalpitch) /2;
        bellOut[2] = bellAmpEnv * bellOut[1];
                
        cowOut[0] = cowFilt[0].lores(cowOsc[0].sinewave(1050 + ( 1771.435 * cowOsc[1].triangle(122.021))+globalpitch), 2500, 3);
        cowOut[1] = cowFilt[1].lores(cowOsc[2].noise(), 7543, 30);
        cowOut[2] = cowEnv * (cowOut[0] + (cowOut[1] * 0.2)) / 3;
        
        ///////////////////////////////////////////////////////////////////////////////
        
        double waveLeft = lowTomOut[2]  + cHatOut[2] +oHatOut[2] +snareOut[2] + kickOut[1] + cowOut[2] +(bellOut[2]*0.8);
        double waveRight = lowTomOut[2] + cHatOut[2] +oHatOut[2] +snareOut[2] + kickOut[1] + cowOut[2] +(bellOut[2]*0.8);
        
        output[i*nChannels    ] = waveLeft; /* You may end up with lots of outputs. add them here */
        output[i*nChannels + 1] = waveRight;
        
        kickTrigger = 0;
        snareTrigger = 0;
        cowTrigger=  0;
        bellTrigger = 0;
        lowTomTrigger = 0;
        oHatTrigger = 0;
        cHatTrigger = 0;
    }
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    changeBeat = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    changeBeat = false;
}
//--------------------------------------------------------------


