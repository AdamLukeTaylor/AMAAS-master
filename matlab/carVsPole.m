%consts
numberOfExps=100;
name='a3b';
carMax=100000;
poleMin=500;
%read pole data
filename=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\aamasResults\\%s\\overall\\final+poleSteps.csv',name)
poleData=csvread(filename);%get data
poleData = poleData(1:(length(poleData)-1));%drop last ellement 0
poleAve=mean(poleData)


%read car data
filename=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\aamasResults\\%s\\overall\\final+carSteps.csv',name)
carData=csvread(filename);%get data
carData = carData(1:(length(carData)-1));%drop last ellement 0
carAve=mean(carData)


%try scan and clip any where car =max
% loopCounter=0;
% while loopCounter<length(carData)%average data
%     
%     if carData(loopCounter+1)==carMax;%accumulate this bin
%         carData=[carData(1:loopCounter),carData(loopCounter+2:length(carData))];
%         poleData=[poleData(1:loopCounter),poleData(loopCounter+2:length(poleData))];
%     else
%         loopCounter=loopCounter+1;
%     end
% end
% %try scan and clip any where car =max
% loopCounter=0;
% while loopCounter<length(poleData)%average data
%     
%     if carData(loopCounter+1)<=poleMin;%accumulate this bin
%         carData=[carData(1:loopCounter),carData(loopCounter+2:length(carData))];
%         poleData=[poleData(1:loopCounter),poleData(loopCounter+2:length(poleData))];
%     else
%         loopCounter=loopCounter+1;
%     end
% end
%make some output arrays
outPoleData=zeros(1,length(poleData)/numberOfExps);%where to ave data
outCarData=zeros(1,length(carData)/numberOfExps);%where to ave data
outCorrData=zeros(1,length(carData)/numberOfExps);%where to ave data
%average pole data it
loopCounter=0;
while loopCounter<length(outPoleData)%average data
    for a=1:numberOfExps
        outPoleData(loopCounter+1)=outPoleData(loopCounter+1)+poleData(loopCounter*numberOfExps+a);%accumulate this bin
    end
    outPoleData(loopCounter+1)=outPoleData(loopCounter+1)/numberOfExps;%average
    loopCounter=loopCounter+1;
end


%average car data  it
loopCounter=0;
while loopCounter<length(outCarData)%average data
    for a=1:numberOfExps
        outCarData(loopCounter+1)=outCarData(loopCounter+1)+carData(loopCounter*numberOfExps+a);%accumulate this bin
    end
    outCarData(loopCounter+1)=outCarData(loopCounter+1)/numberOfExps;%average
    %outCorrData(loopCounter+1)=corr(outPoleData(loopCounter+1),outCarData(loopCounter+1))
    loopCounter=loopCounter+1;
end

%graphiong stuff
plot_type = ['--ro'; '--bx'; '--m+'; '--ks'; '--mx'; '--kx'];
fig1=figure('Visible','on');
hold on
box on
%axisLabelPrint={'00:00','06:00','12:00','18:00','00:00'};
%for full labeled axis
%set(gca,'XLim',[0 100000*10]);%exps for range test
%set(gca,'XTick',[0:1000:10000*1000])
%set(gca,'XTickLabel',axisLabelPrint)
title(name)%'3 tl Cart Pole Vs Mt Car','FontWeight','bold')
xlabel('Runs')
ylabel('Performance')
%legend('Probabilistic','DWL','DWL+PTL(Q+W)', 'Base Load','Location','northwest')


outname=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\AMAAS\\matlab\\output\\MtVsCar.png')
%scatter(locations,data)%,'Color',[1,0,0],'LineStyle','-','Marker','.')

plot(outCarData,'Color',[1,0,0],'LineStyle','-','Marker','.')
plot(outPoleData,'Color',[0,1,0],'LineStyle','-','Marker','.')
legend('Mt Car Steps','Cart Pole Steps','DWL+PTL(Q+W)', 'Base Load','Location','northwest')
%print(fig1,'-dpng','-r1000',outname)
hold off
% 'kendall'
% [r,p]=corr(outCarData(:),outPoleData(:),'type','kendall')
% 'spearman'
% [r,p]=corr(outCarData(:),outPoleData(:),'type','spearman')
% 'pearson'
% [r,p]=corr(outCarData(:),outPoleData(:))
aveRatio=poleAve/carAve
%plot(outCorrData)
