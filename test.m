count = 0; 
count2 = 0; 
problem = 'Huston, we fucked up';
for N = 1:3
    for M = 0:99
        string = strcat(int2str(N),'.',int2str(M));
        [ineq, solvable] = open_files(string);
        res = fourelm(ineq);
        if res ~= solvable
            count = count + 1;
            res
            output
        else
            count2 = count2 + 1;
        end
    end
end
if count > 0
    problem
    count
else
    'segern ?r v?r'
end