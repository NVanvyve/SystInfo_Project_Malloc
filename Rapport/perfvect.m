size = [1 10 25 100 1000 5000 6000 10000 100000];
tc = [ 69 68 84 69 69 67 68 64 132];
tmc =  [ 8 9  9 9 15 48 53 81 868];
tm = [ 1 1 1 1 1 3 3 3 4];
tmm = [ 1 1 1 1 4 5 5 6 9];

figure('Name','Comparaison des temps de réponse de calloc','NumberTitle','off'); 
hold on;
plot(size, tmc, 'k');
plot(size, tc, 'b');
xlabel('Taille demandée [en bytes]');
ylabel('Temps d''exécution [en microsecondes]');
legend('Version du projet','Built-in');
title('Calloc');
grid on;




figure('Name','Comparaison des temps de réponse de malloc','NumberTitle','off'); 
hold on;
plot(size, tmm, 'm');
plot(size, tm, 'r');
xlabel('Taille demandée [en bytes]');
ylabel('Temps d''exécution [en microsecondes]');
legend('Version du projet','Built-in');
title('Malloc');
grid on;