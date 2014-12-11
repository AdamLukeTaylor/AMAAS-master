%pole
scale=100;
numExp=3;
numAlpha=10;
numGamma=10;
output=zeros(numAlpha,numGamma);
alphas=[0,.1,.2,.3,.4,.5,.6,.7,.8,.9];%,1
gammas=[0,.1,.2,.3,.4,.5,.6,.7,.8,.9];%,1];
%read data
filename=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\AMAAS\\results\\expLenghtDuelLonTLon\\overall\\final+poleSteps.csv');
data=csvread(filename);%get data
data = data(1:(length(data)-1));%drop last ellement 0
currentData=1;
for alpha=1:(numAlpha)%plus 1 if using 1.00
    for gamma=1:(numGamma)%plus 1 if using 1.00
        total=0;
        for item=1:numExp
            total=total+data(currentData);%accumulate data
            currentData=currentData+1;
        end
        total=(total/numExp)*scale;
        output(alpha,gamma)=total;%save average
    end
end
%car
carOutput=zeros(numAlpha,numGamma);
%read data
filename=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\AMAAS\\results\\expLenghtDuelLonTLon\\overall\\final+carSteps.csv');
carData=csvread(filename);%get data
carData = carData(1:(length(carData)-1));%drop last ellement 0
currentData=1;
for alpha=1:(numAlpha)%plus 1 if using 1.00
    for gamma=1:(numGamma)%plus 1 if using 1.00
        total=0;
        for item=1:numExp
            total=total+carData(currentData);%accumulate data
            currentData=currentData+1;
        end
        total=(total/numExp)*scale;
        carOutput(alpha,gamma)=1/total;%save average
    end
end
%graphiong stuff
plot_type = ['--ro'; '--bx'; '--m+'; '--ks'; '--mx'; '--kx'];
fig1=figure('Visible','on');
hold on
box on
%axisLabelPrint={'00:00','06:00','12:00','18:00','00:00'};
%for full labeled axis
%set(gca,'XLim',[0 10000*1000]);%exps for range test
%set(gca,'XTick',[0:1000:10000*1000])
%set(gca,'XTickLabel',axisLabelPrint)
title('No TL','FontWeight','bold')
xlabel('Alpha')
ylabel('Gamma')
zlabel('Time Steps');
%legend('Probabilistic','DWL','DWL+PTL(Q+W)', 'Base Load','Location','northwest')
%legend('Original Load','Demand Response','DWL+PTL(Q+W)', 'Base Load','Location','northwest')

outname=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\AMAAS\\matlab\\output\\lenghtGraph.png')
%scatter(locations,data)%,'Color',[1,0,0],'LineStyle','-','Marker','.')

%fig1=subplot(2,1,1);
surf(alphas,gammas,output)
%fig1=subplot(2,1,2);
%surf(alphas,gammas,carOutput)
colormap(summer)
view(3)
%print(fig1,'-dpng','-r1000',outname)

