clear all;
close all;
clc;
more off;

freqAmostragem = 153800       % frequencia de amostragem (PS_8 no Arduino)
freqSinal = 19225             % frequencia do sinal
numeroMaximoDeAmostras = 8;   % numero de pontos coletados

periodoAmostragem = 1.0/freqAmostragem;
periodoSinal = 1.0/freqSinal;
pontosPorCiclo = periodoSinal/periodoAmostragem
numeroDeCiclos = floor(numeroMaximoDeAmostras/pontosPorCiclo)
vetorTempos = 0:periodoAmostragem:periodoSinal*numeroDeCiclos;

seno = sin(2*pi*freqSinal*vetorTempos(1:end-1));
cosseno = cos(2*pi*freqSinal*vetorTempos(1:end-1));

E = [seno' cosseno' ones(length(vetorTempos(1:end-1)),1)];
piE = pinv(E)