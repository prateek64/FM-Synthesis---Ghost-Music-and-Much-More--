# FM SYNTHESIS (GHOST MUSIC AND DJ)


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
