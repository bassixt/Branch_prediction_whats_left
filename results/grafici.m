xp  =[42000,420000,4200000,42000000];
x =[100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 400000000, 405519314];

%%% BIMODAL %%%  WITH 
y_dhry    = [88,  98.09, 93.18, 98.08, 98.98, 96.35, nan, nan, nan]; 
ym_dhry   = [120, 18.99, 68.19, 19.12, 10.12, 36.41, nan, nan, nan]; 
y_LM1     = [62, 65.59, 90.88, 93.05, 93.32, 93.31, nan, nan, nan];
ym_LM1    = [380, 343.99, 91.09, 69.45, 66.77, 66.88, nan, nan, nan];
y_LM10    = [3, 45.5, 84.86, 87.05, 90.85, 92.24, 90.12, 90.53, 90.52];
ym_LM10   = [970, 545, 151.4, 129.44, 91.46, 77.52, 98.71, 94.71, 94.81];
y_core    = [85,  94.09, 92.18, 95.08, 97.52, 96.42, 84.38, nan, nan];
ym_core   = [130, 65.99, 70.12, 41.22, 12.21, 35.79, 156.19, nan, nan];
%y =[97.3,97.5, 99.7,83.6];
%ym =[26.59,24.06,2.88,163.89];
%xq= 0:1:95;
%y_inter = interp1(y,xq,'spline'); %se voglio interpolare usa questa formula


%%%  TAGE  %%%
%z = [98.5,97.8,99.7,89.4];
%zm = [14.66,21.33,2.58,105.99];
z_dhry    = [96, 99.2, 96.9, 98.63, 99.1, 97.47, nan, nan, nan];
zm_dhry   = [40, 8, 31, 13.69, 8.92, 25.26, nan, nan, nan];
z_LM1     = [77, 79, 92.32, 93.35, 93.41, 93.32, nan, nan, nan];
zm_LM1    = [230, 210, 76.8, 66.41, 65.83, 66.73, nan, nan, nan];
z_LM10    = [49, 71.7, 90.31, 90.39, 92.4, 93.68, 92.11, 92.58, 92.57];
zm_LM10   = [510, 283, 96.9, 96.07, 76.04, 63.2, 78.89, 74.1, 74.29];
z_core    = [   95, 98.7,  97.1,  98.2, 98.9, 97.49, 88.90, nan, nan];
zm_core   = [42.27, 10.25, 30.21, 17.7, 10.05,25.06, 110.94, nan, nan];

%set(gca,'FontSize',30)
% ACCURANCY OF OUR PREDICTORS
figure(1)
%plot(xq,y_inter,'r-*');
semilogx(x,y_dhry,'r--o',x,z_dhry,'b--o',x,y_LM1,'m--*',x,z_LM1,'k--*',x,y_LM10,'g--<',x,z_LM10,'y--<',x,y_core,'m-^',x,z_core,'y-^','LineWidth',2);
grid on;
title('Accuracy');
xlabel( 'Number of instructions');
ylabel('Right predictions/Total branch instructions [%]');
legend('Bimodal branch predictor-dhrystone','TAGE branch predictor-dhrystone','Bimodal branch predictor-LONG-MOBILE-1','TAGE branch predictor-LONG-MOBILE-1','Bimodal branch predictor-LONG-MOBILE-10','TAGE branch predictor-LONG-MOBILE-10','Bimodal branch predictor-Coremark','TAGE branch predictor-Coremark');
%MISPREDICTION RATE
figure(2)
semilogx(x,ym_dhry,'r--o',x,zm_dhry,'b--o',x,ym_LM1,'m--*',x,zm_LM1,'k--*',x,ym_LM10,'g--<',x,zm_LM10,'y--<',x,ym_core,'m-^',x,zm_core,'y-^','LineWidth',2);
grid on;
title('Misprediction rate');
xlabel( 'Number of instructions');
ylabel('Number of mispredictions');
legend('Bimodal branch predictor-dhrystone','TAGE branch predictor-dhrystone','Bimodal branch predictor-LONG-MOBILE-1','TAGE branch predictor-LONG-MOBILE-1','Bimodal branch predictor-LONG-MOBILE-10','TAGE branch predictor-LONG-MOBILE-10','Bimodal branch predictor-Coremark','TAGE branch predictor-Coremark');
% % ACCURANCY OF OUR PREDICTORS on particular type of branches
% figure(3)
% plot(x,y,'r-*',x,z,'b-*');
% grid on;
% title('Accuracy on jumps');
% xlabel( 'Number of instructions');
% ylabel('Number of right predictions');
% legend('Bimodal branch predictor','TAGE branch predictor');
% %MISPREDICTION RATE on particular type of branches
% figure(4)
% plot(x,y,'r--o',x,z,'b--o');
% grid on;
% title('Misprediction rate on jump');
% xlabel( 'Number of Mispredictions');
% ylabel('Number of instructions');
% legend('Bimodal branch predictor','TAGE branch predictor');



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Space requirements
s_req    = [1031, 1089, 1205, 2712, 6927, 23330, 37693, 66669, 68010];
figure(3)
semilogx(x,s_req,'r--o','LineWidth',2);
grid on;
title('Data budget');
xlabel( 'Number of instructions');
ylabel('Number of table');
legend('Bimodal branch predictor-LONG-MOBILE-10','TAGE branch predictor-LONG-MOBILE-10');
