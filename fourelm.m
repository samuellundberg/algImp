function res = fourelm(in_sys)

    %Sorterar i v?xande ordning med avseende po forsta variablen, den vi ska eliminera
    system = sortrows(in_sys, 1);    
    [~,n] = size(system);

    %Om vi har mer an en variabel i systemet
    while n > 2
        npos = sum(system(:,1) > 0);      %antalet positiva koefficienter till variabeln 
        nneg = sum(system(:,1) < 0);      %antalet negativa
        nzer = sum(system(:,1) == 0);     %antalet d?r variabeln saknas i systemet

        if npos*nneg + nzer <= 0 % kan uppst?, slut p? olikheter och on?ndligt antal l?sningar
            res = 1;
            return
        end
        %Olikheterna med positiv;negativ koefficient till variabeln vi ska elimenera
        if npos == 0
            non_zero_sys = system(1:nneg,:);
        elseif nneg == 0
            non_zero_sys = system(nzer+1:end,:);
        else
            non_zero_sys = [system(nneg+nzer+1:end,:); system(1:nneg,:)];
        end
        zero_sys = zeros(nzer,n-1); %deklarerar den h?r uppe f?r tydlighet 
        % Vi dividerar vi alla element i raden med avsende po konstanten i forsta kolonnen
        non_zero_sys = non_zero_sys./(non_zero_sys(:,1)*ones(1,n));
        % s? att variabeln vi ska elimenteraa st?lls fri
        non_zero_sys(:, 2:end-1) = non_zero_sys(:, 2:end-1).*-1; 

        new_sys = zeros(npos*nneg + nzer, n-1); %minnesallokerar en matris till olikheterna
        %Nu eliminerar en variabeln och st?ller upp nya olikheter 
        for i = 1:npos
            %Stall upp varje S(a*x)+q<0 mot varje  S(a*x)+q>0, dett ?r v?ra nya
            %olikheter
            new_sys((i-1)*nneg+1:i*nneg,:) = non_zero_sys(npos+1:end,2:end) - ones(nneg,1)*non_zero_sys(i,2:end);
        end
        new_sys(:,end) =  new_sys(:,end) .* -1;         % S(a*x)  < c

        %undviker division med 0 om dessa sparas seprat
        if nzer > 0
            zero_sys = system(nneg+1:nneg+nzer,2:end);  
            new_sys(npos*nneg+1:end, :) = zero_sys;
        end
        system = sortrows(new_sys, 1);
        n = n-1;

    end
    
    % nu slutar vi k?ra algoritmen d? vi bara har en variabel kvar
    if n == 2   % eventuellt on?dig men f?ngar fel
        system = sortrows(system, 1);       %Sorterar med avseende p? f?rsta kolonnen i A (man kan k?ra med sortrows(temp, 1, 'decende') men man m?ste ?nd? sortera om senare
        npos = sum(system(:,1) > 0);      %R?knar antalet positiva konstanter i f?rsta kolonnen
        nneg = sum(system(:,1) < 0);      %R?knar antalet negativa konstanter i f?rsta kolonnen
        nzer = sum(system(:,1) == 0);  
        
        non_zero_sys = [system(nneg+nzer+1:end,:); system(1:nneg,:)];
        zero_sys = system(nneg+1:nneg+nzer,2:end);  %undviker division med 0
        non_zero_sys = non_zero_sys ./ (non_zero_sys(:,1)*ones(1,2));
        
        B1 = inf;
        if npos > 0
            B1 = min(non_zero_sys(1:npos,2));
        end
        b1 = -inf;
        if nneg > 0
            b1 = max(non_zero_sys(npos+1:end,2));
        end
        q_min = 0;
        if nzer > 0
            q_min = min(zero_sys);
        end
        
        if q_min < 0    % 0 < q, q < 0
            res = 0;
            return
        end
        
        if b1<=B1
           res = 1;
           return
        else
            res = 0;
            return

        end
    end
    
    % kommer vi hit ?r n?got fel
    res = -1
end

