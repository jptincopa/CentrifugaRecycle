#!/usr/bin/python
import serial
#arduino = serial.Serial("/dev/ttyUSB0",115200)

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, GObject

import sys  
from datetime import datetime


class SpinnerAnimation(Gtk.Window):

    def __init__(self):

        Gtk.Window.__init__(self, title="Centrifuge GUI ")
        self.set_border_width(3)
        self.connect("delete-event", Gtk.main_quit)
        fixed = Gtk.Fixed()
        
        
        self.label = Gtk.Label()
        self.label.set_markup("<big><b>INTERFAZ DE CONTROL DE CENTRIFUGA</b></big>")
        fixed.put(self.label,150,20)
        self.label2 = Gtk.Label()
        self.label2.set_markup("<big><b>RPM</b></big>")
        fixed.put(self.label2,70,90)
        
        self.label3 = Gtk.Label()
        self.label3.set_markup("<big><b>MINUTOS</b></big>")
        fixed.put(self.label3,550,90)
              
        adjustment = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        spinbutton = Gtk.SpinButton()
        spinbutton.set_adjustment(adjustment)
        fixed.put(spinbutton,550,150)
        
        self.tiempo=Gtk.Label()
        datetimenow = datetime.now()
        fixed.put(self.tiempo,550,250)
                
        self.button = Gtk.ToggleButton("Comenzar \nCentrifugado")
        self.button.set_size_request(100,100)
        self.button.connect("toggled", self.on_button_toggled)
        self.button.connect("clicked", self.on_value_changed, spinbutton)
        self.button.set_active(False)
        fixed.put(self.button,20,150)
        
        self.buttonMas = Gtk.ToggleButton("+")
        self.buttonMas.set_size_request(50,50)
        self.buttonMas.connect("toggled", self.mas_button_toggled)
        self.buttonMas.set_active(False)
        fixed.put(self.buttonMas,120,80)

        self.buttonMenos = Gtk.ToggleButton("-")
        self.buttonMenos.set_size_request(50,50)
        self.buttonMenos.connect("toggled", self.menos_button_toggled)
        self.buttonMenos.set_active(False)
        fixed.put(self.buttonMenos,10,80)
        
        self.spinner = Gtk.Spinner()
        fixed.put(self.spinner,50,300)
        
        self.image=Gtk.Image.new_from_file("cent11.png")
        fixed.put(self.image,0,60)
        
        self.tiempodif=Gtk.Label()
        #self.tiempodif.set_label(str(segundo_act))
        fixed.put(self.tiempodif,550,300)          

        self.add(fixed)
        self.show_all()

    def displayclock(self):
		#  putting our datetime into a var and setting our label to the result. 
		#  we need to return "True" to ensure the timer continues to run, otherwise it will only run once.
		datetimenow = datetime.now()
		self.tiempo.set_label(str(60-datetimenow.second))
		return True

	# Initialize Timer
    def startclocktimer(self):
		#  this takes 2 args: (how often to update in millisec, the method to run)
		GObject.timeout_add(1000, self.displayclock)
	
    def on_value_changed(self, button, spinbutton):
		size = spinbutton.get_value_as_int()
		print size
            		
    def mas_button_toggled(self, button):
		if button.get_active():
			print "lol"#arduino.write('+')
		
		else:
			print "Aumentando Velocidad"

    def menos_button_toggled(self, button):
		if button.get_active():
			print "marmota malcirada"#arduino.write('-')
		
		else:
			print "Bajando Velocidad"		
		
    def on_button_toggled(self, button):		

        if button.get_active():
            self.spinner.start()
            self.button.set_label("PARAR")
            self.image.set_from_file("cent22.png")
            myspinner.startclocktimer()
            
            datetimenow = datetime.now()
            segundo_act=datetimenow.second
            self.tiempodif.set_label(str(60-segundo_act))
            print segundo_act
            

        else:
            self.spinner.stop()
            self.button.set_label("Comenzar \nCentrifugado")
            self.image.set_from_file("cent11.png")
            #arduino.write('0')
     

            
	


myspinner = SpinnerAnimation()
myspinner.connect("delete-event", Gtk.main_quit)
Gtk.main()
