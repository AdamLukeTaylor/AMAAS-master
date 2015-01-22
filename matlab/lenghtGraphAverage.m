%consts
numberOfExps=100;
name='h1a';
%read data
filename=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\aamasResults\\%s\\overall\\final+poleSteps.csv',name)
data=csvread(filename);%get data
data = data(1:(length(data)-1));%drop last ellement 0
outData=zeros(1,length(data)/numberOfExps);%where to ave data
%average it
loopCounter=0;
while loopCounter<length(outData)%average data
    for a=1:numberOfExps
        outData(loopCounter+1)=outData(loopCounter+1)+data(loopCounter*numberOfExps+a);%accumulate this bin
    end
    outData(loopCounter+1)=outData(loopCounter+1)/numberOfExps;%average
    loopCounter=loopCounter+1;
end

%make x points
locations=1:100000:1000000;

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
title('Cart Poll Results','FontWeight','bold')
xlabel('Training Time Steps')
ylabel('Timesteps Before Falling')
%legend('Probabilistic','DWL','DWL+PTL(Q+W)', 'Base Load','Location','northwest')
legend('Original Load','Demand Response','DWL+PTL(Q+W)', 'Base Load','Location','northwest')

outname=sprintf('C:\\Users\\Adam\\Documents\\NetBeansProjects\\AMAAS\\matlab\\output\\lenghtGraph.png')
%scatter(locations,data)%,'Color',[1,0,0],'LineStyle','-','Marker','.')
plot(outData,'Color',[1,0,0],'LineStyle','-','Marker','.')
%print(fig1,'-dpng','-r1000',outname)

