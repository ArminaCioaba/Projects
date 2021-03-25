function [X,Y]=RungeKutta4(f,a,b,y0,n) %Apelul si declararea functiei
%a si b reprezinta capetele intervalului, y0 reprezinta conditia initiala,
%iar n reprezinta numarul de iteratii
tic;
h=(b-a)/n; %Aflarea pasului
t=a:h:b; %Initializarea domeniului de valori
Y=zeros(n,1); %Alocarea de spatiu lui Y de n linii si o coloana
X=[a:h:b]' %Transpunerea lui X (in X se afla abscisele)
Y(1,1)=y0; %Initializarea lui y0

% Aplicarea algoritmului Runge-Kutta de ordin 2
for i=2:(length(t))
    k1=h*f(X(i-1,1),Y(i-1,1));
    k2=h*f(X(i-1,1)+h/2,Y(i-1,1)+k1/2);
    k3=h*f(X(i-1,1)+h/2,Y(i-1,1)+k2/2);
    k4=h*f(X(i-1,1)+h/2,Y(i-1,1)+k3/2);
 % Aflarea valorilor functiei in x
     Y(i,1)=Y(i-1,1)+(k1+2*(k2+k3)+k4)/6;
end
Y
% Aflarea timpului de executie al algoritmului Runge-Kutta de ordin 2
timprungekutta4=toc
% Aflarea timpul de executie al metodei ode23
tic; 
[xval,yval] = ode45(f,t,y0)
timpode45=toc
% Diferenta
diferenta=Y-yval;
% Eroarea
eroare=norm(diferenta)
%Reprezentarea grafica a rezultatelor obtinute folosind functia ode23
%cu negru
%Reprezentarea grafica a rezultateloer obtinute folosind algoritmul
%Runge-Kutta de ordin 4 cu rosu
plot(xval,yval,'b',X,Y,'r--')
pause;