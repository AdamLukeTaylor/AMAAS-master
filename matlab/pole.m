%consts
numberOfExps=1;
name='h1a';
runName='25';
%read pole data
%filename=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\aamasResults\\%s\\overall\\final+poleSteps.csv',name);
filename=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\aamasResults\\%s\\overall\\run %s+poleSteps.csv',name,runName);
poleData=csvread(filename);%get data
poleData = poleData(1:(length(poleData)-1));%drop last ellement 0
poleAve=mean(poleData)
outPoleData=zeros(1,length(poleData)/numberOfExps);%where to ave data
%average it
loopCounter=0;
while loopCounter<length(outPoleData)%average data
    for a=1:numberOfExps
        outPoleData(loopCounter+1)=outPoleData(loopCounter+1)+poleData(loopCounter*numberOfExps+a);%accumulate this bin
    end
    outPoleData(loopCounter+1)=outPoleData(loopCounter+1)/numberOfExps;%average
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
title(sprintf('Run number = %s',runName),'FontWeight','bold')
%xlabel('Mt Car Steps')
%ylabel('Cart Pole Steps')
%legend('Probabilistic','DWL','DWL+PTL(Q+W)', 'Base Load','Location','northwest')


outname=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\AMAAS\\matlab\\output\\MtVsCar.png')
%scatter(locations,data)%,'Color',[1,0,0],'LineStyle','-','Marker','.')
plot(outPoleData,'Color',[0,1,0],'LineStyle','-','Marker','.')
%legend('Mt Car Steps','Cart Pole Steps','DWL+PTL(Q+W)', 'Base Load','Location','northwest')
%print(fig1,'-dpng','-r1000',outname)
