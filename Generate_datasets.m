

close all 
clear


n   = 10 ;
dim = 2  ;
k   = 5  ;
scaling = 10 ; %Range of values for the coordinates of the points  [-scaling,scaling]


if exist( "knn_datasets_zero_baced2" , "dir") ~= 7                
        file_name = mfilename;
        path_with_code = mfilename('fullpath');
        path_to_code = path_with_code(1:length(path_with_code)-length(file_name));
        fprintf('The folder "knn_datasets_zero_baced" does not exist in the current directory.\n' );
        fprintf('The current directory is : %s.\n', path_to_code );
        return;
end

%filename = 'knn_dataset6.txt' ;        % example 
datasetnumber = 0 ;
filename = 'knn_datasets_zero_baced/knn_dataset' + string(datasetnumber) + '.txt' ; 

while exist( filename , 'file') == 2 
        %This means the file exists
        datasetnumber = datasetnumber + 1 ;
        filename = 'knn_datasets_zero_baced/knn_dataset' + string(datasetnumber) + '.txt' ; 
end


fprintf('The file was named : %s \n' , filename);


if k >= n
        k = n-1 ;
end

X = scaling*rand(n, dim)-scaling/2;
Y = X ;

D = (sum(X.^2,2) - 2 * X*Y.' + sum(Y.^2,2).');

[~,I] = mink(D,k+1);

% Plots the points using the first 2 dimentions
%figure; 
%plot(X(:,1),X(:,2),"*")


% make them zero-baced
I=I-1 ;
start = [n dim k ] ;            % This is the first row of the file
extra = [X  I(2:k+1,:)'];       % Those are the data for the file


I(2:k+1,:)';


fprintf('The file was named : %s \n' , filename);

writematrix(start,filename)
writematrix(extra,filename,'WriteMode','append')


% This will read only the matrix not the first row 
%readmatrix(filename)









