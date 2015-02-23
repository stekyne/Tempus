Tempus
======

A C++ based audio plugin for time orientated audio effects. Built using the Juce framework.

Features (work in progress):
 - Basic variable delays
 - Delay feedback
 - Modulated delay lines (chorus, flange, phaser effects)

TODO:
 - UI needs to be completed
 - Stereo widening
 - More processing of feedback, soft clipping, distortion, filtering
 - More post processing, reverb, granular, pitch shifting
 - Smoother audio delay lines, click free always
 - Lockfree audio callback

Possibly base implementation on this paper:
http://www.dafx14.fau.de/papers/dafx14_vesa_norilo_exploring_the_vectored_ti.pdf
