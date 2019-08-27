#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
import numpy as np
import serial
import atexit
import struct

win = pg.GraphicsWindow()
win.setWindowTitle('Amplitude medida')
p1 = win.addPlot()
data1 = np.zeros(300)
curve1 = p1.plot(data1, pen='r')
ptr1 = 0
ponto = 0

conexaoSerial = serial.Serial('/dev/ttyUSB0',115200)

def saindo():
    print('Saindo')

def update():
    global data1, curve1, ptr1, conexaoSerial, ponto
    if conexaoSerial.inWaiting() > 1:
        ieee754_data = conexaoSerial.read(4)     # le 4 bytes da serial
        dado = struct.unpack('f', ieee754_data)  # converte para float
        data1[ponto] = dado[0];             # atualiza vetor do grafico
        ponto = ponto + 1
        if ponto > 299:
            ponto = 0
        curve1.setData(data1)               # atualiza grafico
        
# inicia timer rodando o mais rápido possível
timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(0)

# registra funcao de saida
atexit.register(saindo)

## Start Qt event loop
QtGui.QApplication.instance().exec_()
