function [ineq, solution] = open_files(filenum)   
file = filenum;
rA = fscanf(fopen(strcat('A',file)), '%d');
rc = fscanf(fopen(strcat('c',file)), '%d');
output = fscanf(fopen(strcat('output',file,'.txt')), '%s');
if (strcmp(output, 'False')==1)
    solution = 0;
else 
    solution = 1;
end
fclose('all');
rows = rA(1);
cols = rA(2);
rA = rA(3:end);
A = zeros(rows, cols);
i = 0; 

for R=1:rows
    for C=1:cols
        i =i+1;
        A(R,C) = rA(i);
    end
end
c = rc(2:end);

ineq = [A c];
end