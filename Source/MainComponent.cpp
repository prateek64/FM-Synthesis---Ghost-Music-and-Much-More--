/* 

VERY IMPORTANT:: NOTE** - Before running the application change the CURRENT_DIR variable (in the MainContentComponent constructor) to the directory where the
"Source" files for your JUCE application reside.



INTRODUCTION: This audio application demonstrates basics of sine wave fm synthesis and makes music too!! (ghost music and DJ!!).
With this audio application you can do many cool things. The following are the controls using which the user can experience the various features of the 
app.

1. Manual Mode: By default you have a manual mode , which gives you the following controls : 

		a) Switch On and Switch Off - You can control whether you want to hear the FM synthesis or not by using the Switch On and Switch Off button.

		b) Sliders: We have provided you with four sliders to control the following: Carrier Frequency, Carrier Gain, Modulator Frequency and Modulator Gain.
		   These are manual controls so the user can control these by mouse or by hand (in case of touchscreen) and see the effects of a modulator on a carrier.

		c) Ghost Music Mode: This is a music mode that the user controls by using the Up, Down, Left and Right key presses. Each key press generates a new 
		   value for the four parameters (Carrier Frequency, Modulator Frequency, Modualtor Gain and Carrier Gain) and makes music. Go Experiment!!

		d) Make It Weirder : This Mode generates new patterns of modulations using multiple sine waves (maximum three for our application). Each modulator's
		   frequency is linked to the others using some mapping so it generates a complex feedback modulation. Then this modulation is applied to the main 
		   carrier wave. This generates really weird FM sythesis patterns. 


2. Press For Fun Mode: This is probably the coolest modes of all described above. This is the DJ music mode. This mode lets you be a DJ (a very bad one!
   honestly) on your laptop. The mode does the following : 

		a) Control Drum Beats : You can play the drums(different beats) using the A, S, D, H, and L keypresses. 

		b) DJ Disc: Play the DJ Disc by clicking and dragging the mouse over the disc( in the audio application). It is best suited for a touchsreen as 
		   it makes the gesture of playing thr DJ disc even more apt. (See project video for how it works).

		c) Crash Sound: This sound is used to culminate the sound piece. Use the crash sound by using the Space key.

*/


#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sine.h"
#include "Smooth.h"
#include "FaustReverb.h";

using namespace std;

class MainContentComponent :
    public AudioAppComponent,
    private Slider::Listener,
    private ToggleButton::Listener,  // Listens to ToggleButton presses
	private KeyListener  // Listens to the Key Presses
{

public:
    MainContentComponent()
    : carrierFrequency(440.0), modulatorFrequency(20.0),  // Initialises the carrier and modulator frequency, modulator gain and OnOff button state
    index(0.0),
    onOff (0.0)
    {
		
		String CURRENT_DIR = "C:/Users/pmurgai/Desktop/FM/Source"; // Directory for the project source files, please change according to your system.

		set_current_dir(CURRENT_DIR); // Sets the current directory variable

		sliders_N_buttons_init();     // Contains functions that initialize the sliders and buttons.
		labels_init();                // Initializes all the labels for the application. 

		setSize(600, 600);                // Sets the screen size

		nChans = 2;                       // Number of output channels

        setAudioChannels (0, nChans);    //  No inputs, two outputs

		formatManager.registerBasicFormats(); // Registers basic audio formats/

		audioBuffer = new float*[nChans];    // Initializes an audiobufferr

		file_name_collection();   // Contains all the filenames for the files used in the "Press For More Fun" mode.
    }



	// Sets the current directory variable which is later used to create absolute paths for files used in the "Press For More Fun" mode

	void set_current_dir(String dir) {


		current_dir = dir;

	}



	// Adds and makes all the buttons visible on the application screen. It also adds listener for each of the button

	void buttons_initialization() {

	
		addAndMakeVisible(onOffButton);
		onOffButton.setButtonText("Switch On");
		onOffButton.addListener(this);
		onOffButton.setColour(0x1000100, Colours::blueviolet);
		
		addAndMakeVisible(moreFun);
		moreFun.setButtonText("Press For More Fun");
		moreFun.addListener(this);
		moreFun.setColour(0x1000100,Colours::aquamarine);

		addAndMakeVisible(offButton);
		offButton.setButtonText("Switch Off");
		offButton.addListener(this);
		onOffButton.setColour(0x1000100, Colours::darkcyan);
		offButton.setVisible(false);

		addAndMakeVisible(ghost_music_mode);
		ghost_music_mode.setButtonText("Ghost Music Mode");
		ghost_music_mode.addListener(this);
		ghost_music_mode.setColour(0x1000100,Colours::aliceblue);

		addAndMakeVisible(make_it_weirder);
		make_it_weirder.setButtonText("Make It Weirder");
		make_it_weirder.addListener(this);
		make_it_weirder.setColour(0x1000100, Colours::firebrick);



	}


	// Adds and makes visible all the labels on the application screen.

	void labels_init() {

		// configuring carrier label box and adding it to the main window
		addAndMakeVisible(carrierLabel);
		carrierLabel.setText("Carrier Freq", dontSendNotification);
		carrierLabel.attachToComponent(&carrierSlider, false);

		// configuring modulator label box and adding it to the main window
		addAndMakeVisible(modulatorLabel);
		modulatorLabel.setText("Modulator Freq", dontSendNotification);
		modulatorLabel.attachToComponent(&modulatorSlider, false);

		// configuring carrier label box and adding it to the main window
		addAndMakeVisible(indexLabel);
		indexLabel.setText("M Gain", dontSendNotification);
		indexLabel.attachToComponent(&indexSlider, false);

		addAndMakeVisible(gainLabel);
		gainLabel.setText("C Gain", dontSendNotification);
		gainLabel.attachToComponent(&gainSlider, false);


	}


	// Calls all the slider and button initialization buttons. It also adds a key listener to the main window component

	void sliders_N_buttons_init() {

		getTopLevelComponent()->addKeyListener(this);
		// configuring frequency sliders and adding them to the main window

		carrier_slider_initialization();

		// configuring frequency slider and adding it to the main window
		modulation_slider_initialization();

		index_slider_initialization();

		gain_slider_initialization();

		buttons_initialization();
	}


	// Adds and makes visible the carrier gain slider
    
	void gain_slider_initialization() {

		// configuring gain slider and adding it to the main window
		addAndMakeVisible(gainSlider);
		gainSlider.setSliderStyle(Slider::LinearBarVertical);
		gainSlider.setColour(0x1001200, Colours::lightcoral);
		gainSlider.setRange(0.0, 1.0);
		gainSlider.setValue(0.5);
		gainSlider.addListener(this);


	}

	// Adds and makes visible the modulator gain slider

	void index_slider_initialization() {

		addAndMakeVisible(indexSlider);
		indexSlider.setSliderStyle(Slider::LinearBarVertical);
		indexSlider.setColour(0x1001200, Colours::crimson);
		indexSlider.setRange(0.0, 1000.0);
		indexSlider.setValue(0.0);
		indexSlider.addListener(this);

	}

	// Adds and makes visible the carrier frequency slider

	void carrier_slider_initialization() {

		addAndMakeVisible(carrierSlider);
		carrierSlider.setSliderStyle(Slider::LinearBarVertical);
		carrierSlider.setColour(0x1001200, Colours::cyan);
		carrierSlider.setRange(50.0, 8000.0);
		carrierSlider.setSkewFactorFromMidPoint(500.0);
		carrierSlider.setValue(1000);
		carrierSlider.addListener(this);

	}


	// Adds and makes visible the modulator frequency slider

	void modulation_slider_initialization() {

		addAndMakeVisible(modulatorSlider);
		modulatorSlider.setSliderStyle(Slider::LinearBarVertical);
		modulatorSlider.setColour(0x1001200, Colours::darkkhaki);
		modulatorSlider.setRange(1.0, 5000.0);
		modulatorSlider.setSkewFactorFromMidPoint(500.0);
		modulatorSlider.setValue(50);
		modulatorSlider.addListener(this);

	}


	// Contains all the filenames used in the "Play For More Fun" mode.

	void file_name_collection() {

		file_names[0] = current_dir + "/DataFiles/drum_1.wav";
		file_names[1] = current_dir + "/DataFiles/drum_3.wav";
		file_names[2] = current_dir + "/DataFiles/drum_2.wav";
		file_names[3] = current_dir + "/DataFiles/crash.wav";
		file_names[4] = current_dir + "/DataFiles/djnoise2.wav";
		file_names[5] = current_dir + "/DataFiles/disc_4.jpg";
		file_names[6] = current_dir + "/DataFiles/unicorn_fart.jpg";
		file_names[7] = current_dir + "/DataFiles/unicorn_fart.wav";
		file_names[8] = current_dir + "/DataFiles/drum_4.wav";
		file_names[9] = current_dir + "/DataFiles/drum_5.wav";
		
	}


	// This methods is used to load the files into a file buffer which is later used in the audio callback function

	void load_sounds(String filename) {

		File file = (filename);

		ScopedPointer<AudioFormatReader> reader(formatManager.createReaderFor(file)); 

		if (reader != nullptr)
		{
			const double duration = reader->lengthInSamples / reader->sampleRate;     

			if (duration < 5)  // Checks if the duration of the file is less than five seconds
			{
				fileBuffer.setSize(reader->numChannels, reader->lengthInSamples);      
				reader->read(&fileBuffer,                                              
					0,                                                        
					reader->lengthInSamples,                                  
					0,                                                        
					true,                                                     
					true);                                                    
				position = 0;                                                           
				setAudioChannels(0, reader->numChannels);                              
			}

			else

			{


			}

		}


		file.~File();

	}


	// Function which changes the color of all the components in the application based on the number of keypresses in the ghost music mode. 

	void change_color() {

		if (keypress_count >= 0 && keypress_count < 4) {

			carrierSlider.setColour(0x1001200, Colours::chartreuse);
			modulatorSlider.setColour(0x1001200, Colours::deepskyblue);
			indexSlider.setColour(0x1001200, Colours::lavender);
			gainSlider.setColour(0x1001200, Colours::deeppink);
			make_it_weirder.setColour(0x1000100, Colours::magenta);
			ghost_music_mode.setColour(0x1000100, Colours::navy);

		}

		else if (keypress_count >= 4 && keypress_count < 8) {

			carrierSlider.setColour(0x1001200, Colours::lightskyblue);
			modulatorSlider.setColour(0x1001200, Colours::indigo);
			indexSlider.setColour(0x1001200, Colours::linen);
			gainSlider.setColour(0x1001200, Colours::chartreuse);
			make_it_weirder.setColour(0x1000100, Colours::dodgerblue);
			ghost_music_mode.setColour(0x1000100, Colours::firebrick);

		}

		else if (keypress_count >= 8 && keypress_count < 12) {

			carrierSlider.setColour(0x1001200, Colours::aqua);
			modulatorSlider.setColour(0x1001200, Colours::bisque);
			indexSlider.setColour(0x1001200, Colours::lavenderblush);
			gainSlider.setColour(0x1001200, Colours::lightsteelblue);
			make_it_weirder.setColour(0x1000100, Colours::royalblue);
			ghost_music_mode.setColour(0x1000100, Colours::whitesmoke);

		}

		else if (keypress_count >= 12 && keypress_count < 16) {

			carrierSlider.setColour(0x1001200, Colours::darkolivegreen);
			modulatorSlider.setColour(0x1001200, Colours::lemonchiffon);
			indexSlider.setColour(0x1001200, Colours::lightpink);
			gainSlider.setColour(0x1001200, Colours::mediumblue);
			make_it_weirder.setColour(0x1000100, Colours::mediumaquamarine);
			ghost_music_mode.setColour(0x1000100, Colours::mediumspringgreen);

		}

		else if (keypress_count >= 16 && keypress_count < 20) {

			carrierSlider.setColour(0x1001200, Colours::darkturquoise);
			modulatorSlider.setColour(0x1001200, Colours::greenyellow);
			indexSlider.setColour(0x1001200, Colours::mistyrose);
			gainSlider.setColour(0x1001200, Colours::olive);
			make_it_weirder.setColour(0x1000100, Colours::papayawhip);
			ghost_music_mode.setColour(0x1000100, Colours::orchid);

		}

		else if(keypress_count > 20) {

			keypress_count = 0;
			
		}
		

	}
	


	// Callback function which is called when a key is pressed

	bool keyPressed(const KeyPress &key, Component*originatingComponent) override {

		const String A = "a";  // Used later to compare if the A key is pressed
		const String S = "s";  // Used later to compare if the S key is pressed
		const String D = "d";  // Used later to compare if the D key is pressed
		const String H = "h";  // Used later to compare if the H key is pressed
		const String L = "l";  // Used later to compare if the L key is pressed

		if (ghost_mode == 1) {     // Checks these key presses only if ghost mode is activated. 

			change_color();

			

			if (key.isKeyCode(key.leftKey)) {   // Checks if leftkey is presses

				carrierSlider.setValue(1500);
				modulatorSlider.setValue(2.0);
				indexSlider.setValue(100);
				keypress_count++;

			}

			if (key.isKeyCode(key.upKey)) {

				set_smoothing_value(0.99999);

				carrierSlider.setValue(carrierSlider.getMaximum()*random.nextDouble() );
				modulatorSlider.setValue(modulatorSlider.getMaximum()*random.nextDouble());
				indexSlider.setValue(indexSlider.getMaximum()*random.nextDouble());
				keypress_count++;

			}

			if (key.isKeyCode(key.downKey)) {


				set_smoothing_value(0.999);
				carrierSlider.setValue(carrierSlider.getMaximum()*random.nextDouble());
				modulatorSlider.setValue(modulatorSlider.getMaximum()*random.nextDouble());
				indexSlider.setValue(indexSlider.getMaximum()*random.nextDouble());
				keypress_count++;


			}

			if (key.isKeyCode(key.rightKey)) {

				
				carrierSlider.setValue(500);
				modulatorSlider.setValue(8.0);
				indexSlider.setValue(200);
				keypress_count++;


			}

			if (key.isKeyCode(key.spaceKey)) {

				carrierSlider.setValue(carrierSlider.getMaximum()*random.nextDouble());
				modulatorSlider.setValue(modulatorSlider.getMaximum()*random.nextDouble());
				indexSlider.setValue(indexSlider.getMaximum()*random.nextDouble());
				keypress_count++;


			}

		}


		if (more_fun == 1) {  // Checks these key presses only if in the "Press For More Fun Mode"

			if (A.containsChar(key.getTextCharacter())) {   // Checks if the A key is pressed

				song_count = 1;
				shutdownAudio();        // Shuts down any audio that might be playing/loaded.                                                              
				load_sounds(file_names[0]);
			}


			if (D.containsChar(key.getTextCharacter())) {

				song_count = 1;
				shutdownAudio();                                                                   
				load_sounds(file_names[1]);

			}

			if (L.containsChar(key.getTextCharacter())) {

				song_count = 1;
				shutdownAudio();                                                                    
				load_sounds(file_names[9]);

			}

			if (key.isKeyCode(key.spaceKey)) {   // Checks if the space key is pressed

				song_count = 1;
				shutdownAudio();                                                                    
				load_sounds(file_names[3]);
			}

			if (H.containsChar(key.getTextCharacter())) {

				song_count = 1;
				shutdownAudio();                                                                   
				load_sounds(file_names[2]);
				

			}
			
			if (S.containsChar(key.getTextCharacter())) {
				
				song_count = 1;
				shutdownAudio();                                                                    // [1
				load_sounds(file_names[8]);

			} 
		}

		return true;
	}



	// Initializes the DJ disc image for the "Press For More Fun"
	void image_init() {

		File file(file_names[5]);
		img = ImageFileFormat::loadFrom(file);
		file.~File();

	}


	// // Initializes the farting unicorn image

	void unicorn_fart_init() {

		File file_fart(file_names[6]);
		img_unicorn_fart = ImageFileFormat::loadFrom(file_fart);
		file_fart.~File();
	    
	}



	// Callback function which draws all the graphics on the application window
	void paint(Graphics &g) override {

		if (draw_img == 0) {
			g.fillAll(Colours::darkgrey);
			unicorn_fart_init();
			// img.rescaled(30,30);
			g.drawImageAt(img_unicorn_fart, 215, 180);  // Draws the farting unicorn image on the application window
		
		}


		if (draw_img == 1) {
			image_init();
			g.fillAll(Colours::black);
		    onOffButton.setBounds(700, 700, 300,300);
			g.drawImageAt(img,50,50);   // Draws the DJ disc image on the application window
			
			
		}

	}

    ~MainContentComponent()
    {
		fileBuffer.setSize(0, 0);  // Sets the size of the filebuffer to 0
        shutdownAudio();
		delete[] audioBuffer;  // deletes the audio buffer
		
    }
    

	// Callback function when the window is resized

    void resized() override
    {
       
        const int sliderLeft = 10;
        onOffButton.setBounds (sliderLeft+10 , getHeight() - 150 + 60, 80, 40);
		offButton.setBounds(sliderLeft+10, getHeight() - 150 + 60, 80, 40);

		carrierSlider.setBounds(sliderLeft, 40, 100, getHeight() - 150);
		modulatorSlider.setBounds(getWidth() - sliderLeft - 100, 40, 100, getHeight() - 150);
		indexSlider.setBounds(getWidth() - sliderLeft - 100 - 80, 40, 70, getHeight() - 150);
		gainSlider.setBounds(sliderLeft + 110, 40, 70, getHeight() - 150);

		moreFun.setBounds(sliderLeft +215, getHeight() / 2 - 200, 150, 50);
		
		ghost_music_mode.setBounds(sliderLeft + 100+20, getHeight() - 150 + 50, 150, 50);
		make_it_weirder.setBounds(sliderLeft + 215 + 100, getHeight() - 150 + 50, 150, 50);

		stop_DJ.setBounds(getWidth() - 80, getHeight() - 60, 60, 50);
		start_DJ.setBounds(getWidth() - 80, getHeight() - 60, 60, 50);


    }


	// Callback function called when any of the sliders are moved
    
    void sliderValueChanged (Slider* slider) override
    {
        if (carrier.getSamplingRate() > 0.0){

            if (slider == &carrierSlider)

            {

                carrierFrequency = carrierSlider.getValue();  // Sets the carrier frequency 

            }

            else if (slider == &modulatorSlider)

            {

				modulatorFrequency = modulatorSlider.getValue();
                
            }

            else if (slider == &indexSlider)

            {

                index = indexSlider.getValue();   // Sets the modulator gain

            }

        }
    }



	// Callback function called when any of the buttons are pressed
    
    void buttonClicked (Button* button) override
    {
        if(button == &onOffButton) {    // Checks if the Switch On button is pressed

            onOff = 1.0;
			onOffButton.setVisible(false);
			offButton.setVisible(true);

        }
        
		if (button == &offButton) {   // Checks if the switch off button is pressed

			onOff = 0;
			offButton.setVisible(false);
			onOffButton.setVisible(true);

		}

		if (button == &moreFun) {   // Checks if the "Press For More Fun" button is pressed
			
			ghost_mode = 0;
			initial_file_read();
			more_fun = 1;
			draw_img = 1; 
		    repaint();
			make_invisible();
			stop_dj();
			start_dj();
            	
			
		}


		if (button == &ghost_music_mode) {   // Checks if the Ghost Music Mode Button is Pressed

			ghost_mode = 1;

		}

		if (button == &make_it_weirder) {

			modulator_count++;
		}
		
		if (button == &stop_DJ) {

			more_fun = 2; 
			stop_DJ.setVisible(false);
			start_DJ.setVisible(true);
			
		}

		if (button == &start_DJ) {

			more_fun++;
			start_DJ.setVisible(false);
			stop_DJ.setVisible(true);
			more_fun = 1;


		}


    }

	void stop_dj() {

		addAndMakeVisible(stop_DJ);
		stop_DJ.setButtonText("Stop DJ");
		stop_DJ.addListener(this);
	}

	void start_dj() {

		addAndMakeVisible(start_DJ);
		start_DJ.setButtonText("Start DJ");
		start_DJ.addListener(this);
		start_DJ.setVisible(false);

	}

	// Callback function which is called when the mouse button is pressed and down.

	void mouseDown(const MouseEvent& e) {

		if (more_fun == 1) {

			song_count = 1;
			shutdownAudio();                                                                    // [1
			load_sounds(file_names[4]);

		}

	}


	// Makes all the sliders and buttons invisible for the "Press For More Fun" mode.

	void make_invisible() {

		moreFun.setVisible(false);
		carrierSlider.setVisible(false);
		modulatorSlider.setVisible(false);
		indexSlider.setVisible(false);
		gainSlider.setVisible(false);
		onOffButton.setVisible(false);
		ghost_music_mode.setVisible(false);
		make_it_weirder.setVisible(false);
		offButton.setVisible(false);
		
	}

	// Plays the unicorn fart sound when the  "Press For More Fun" button is pressed
    
	void initial_file_read() {

		song_count = 1;
		shutdownAudio();
		load_sounds(file_names[7]);

		
	}



	
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {

        carrier.setSamplingRate(sampleRate);   // Sets the sampling rate for the carrier 
        carrier.setFrequency(carrierFrequency);

        modulator.setSamplingRate(sampleRate);
		modulator_1.setSamplingRate(sampleRate);

        modulator.setFrequency(50.0);  // Sets the frequencies of the modulators
		modulator_1.setFrequency(80);

		set_smoothing_value(0.999); 
       

		reverb.init(sampleRate);
		reverb.buildUserInterface(&reverbControl);
    }
    


	// Sets the smoothing values 

	void set_smoothing_value(double value) {

		for (int i = 0; i < 7; i++) {      // Initializes the smoothing function values

			smooth[i].setSmooth(value);

		}

	}




    void releaseResources() override
    {
		fileBuffer.setSize(0, 0);
		shutdownAudio();
    }
    

	// Audio Callback Function 

	/*

	This audio callback function fills audio buffers in two different ways. If the "Press For More Fun" mode is not active it peforms sine wave FM synthesis.
	In case the "Press For More Fun" mode is active it reads data from wav files given in the file_names function and doesn't perform the sine wave synthesis
	
	*/


    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
		
		if (more_fun == 0) {                                 // Checks if the "Press For More Fun" mode is active

			const float level = gainSlider.getValue();      //  Gain is updated every block
		
			for (int i = 0; i < nChans; i++) {

				audioBuffer[i] = bufferToFill.buffer->getWritePointer(i, bufferToFill.startSample); 

			}


			// computing one audio block

			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
			
				if (modulator_count == 0) {  // Checks if the "Make it Weirder button has been pressed zero times

					modulator.setFrequency(smooth[4].tick(modulatorFrequency));
					carrier.setFrequency(smooth[0].tick(carrierFrequency) + modulator.tick()*smooth[3].tick(index)); // modulating freq of the carrier
				}

				else if (modulator_count == 1) {   // Checks if the "Make it Weirder button has been pressed one time

					modulator.setFrequency(smooth[4].tick(modulatorFrequency) + modulator_1.tick()*smooth[5].tick(index/1.5));
					carrier.setFrequency(smooth[0].tick(carrierFrequency) + modulator.tick()*smooth[3].tick(index)); // modulating freq of the carrier

				}

				else if (modulator_count == 2) {  // Checks if the "Make it Weirder button has been pressed two times

					modulator_1.setFrequency(250 + modulator.tick()*smooth[6].tick(index));
					modulator.setFrequency(smooth[4].tick(modulatorFrequency) + modulator_1.tick()*smooth[5].tick(index / 1.5));
					carrier.setFrequency(smooth[0].tick(carrierFrequency) + modulator.tick()*smooth[3].tick(index)); // modulating freq of the carrier

				}

				audioBuffer[0][sample] = carrier.tick() * smooth[1].tick(level) * smooth[2].tick(onOff); // computing sample
				

			}

			reverb.compute(bufferToFill.numSamples, audioBuffer, audioBuffer);
		}

		if (more_fun == 1) {   // Checks if the "Press For More Fun" is active  

			
			const int numInputChannels = fileBuffer.getNumChannels();
			const int numOutputChannels = bufferToFill.buffer->getNumChannels();

			int outputSamplesRemaining = bufferToFill.numSamples;                                   
			int outputSamplesOffset = bufferToFill.startSample;                                     

			while (outputSamplesRemaining > 0)

			{
				int bufferSamplesRemaining = fileBuffer.getNumSamples() - position;                 
				int samplesThisTime = jmin(outputSamplesRemaining, bufferSamplesRemaining);        

				for (int channel = 0; channel < numOutputChannels; ++channel)
				{
					bufferToFill.buffer->copyFrom(channel,                                         
						outputSamplesOffset,                            
						fileBuffer,                                     
						channel % numInputChannels,                      
						position,                                        
						samplesThisTime);                                
				}



				outputSamplesRemaining -= samplesThisTime;                                       
				outputSamplesOffset += samplesThisTime;                                            
				position += samplesThisTime;                                                        

				if (song_count == 1) {

					if (position == fileBuffer.getNumSamples())

						position = 0;
				}

			}

			song_count = 0;
		

		}

		if (more_fun > 1) {

			fileBuffer.setSize(0, 0);
			shutdownAudio();
		}

		
    }
    
    
private:

	String current_dir= "";


	String file_names[10]; // String containing the file_names
	Image img,img_unicorn_fart; // Image objects 
    Slider carrierSlider;  // Slider for controlling the carrier frequency
    Slider modulatorSlider;
    Slider indexSlider;   // Slider controlling the modulator gain 
    Slider gainSlider;
    
    
	// Label initialization

    Label carrierLabel;
    Label modulatorLabel;
    Label indexLabel;
    Label gainLabel;
    
	// Initializes all the text buttons

	TextButton moreFun;
	TextButton start_DJ;
	TextButton stop_DJ;
	TextButton onOffButton;
	TextButton offButton;
	TextButton make_some_music;
	TextButton ghost_music_mode;
	TextButton make_it_weirder;


	// Initializes the carriers and modulators variables

    Sine carrier, modulator,modulator_1;
    Smooth smooth[7]; // to prevent cliking of some of the parameters

    

    double carrierFrequency,modulatorFrequency, index, onOff;

	
	FaustReverb reverb;    // Reverb object 
	MapUI reverbControl;


	AudioFormatManager formatManager;
	AudioSampleBuffer fileBuffer;

	int nChans; // Number of Audio Inputs/Outputs
	int keypress_count = 0;
	int position, song_count = 1, more_fun = 0,draw_img = 0, mouse_down = 0,ghost_mode = 0, modulator_count = 0;

	float** audioBuffer;  // Audio Buffer Variable 

	Random random;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
