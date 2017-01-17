clc
clear

% Q=[53.5 52.5 -84.5 2.75 5.5 -8 -127.25 49.5 47;
%     -161.5 42.5 -108.5 6.75 -9.5 1 -110.25 49.5 -168;
%      24.5 -141.5 101.5 -16.25 31.5 -2 115.75 -148.5 56;
%      83.5 46.5 91.5 6.75 -27.5 9 121.75 49.5 65.0]'
% Cv=Q*Q';
% [V,D]=eig(Cv); 

% Read Training Images
n = 48; size =10304;
x = zeros(size, n);

for j=1:6
    for i=1:8
        file_name = ['PCA_DB\\Training\\set_' num2str(j) '\\' num2str(i) '.bmp'] 
        input_image = double(rgb2gray(imread(file_name)));     
        inputT = input_image';
        x(:, (j-1)*8 + i) = inputT(:);    
    end
end

m = zeros(size,1);
m(:) = mean(x');
for i=1:48
    x(:,i) = x(:,i)-m(:);
end

%Covariance Matrix
c=x*x';
%Eigen Vector, Eigen Value
[v, d] = eig(c);

for i=1:10
    file_name = ['eigenvector' num2str(i) '.txt'];
    fid_v = fopen(file_name,'w'); % ctrl r ctrl t    
    fprintf(fid_v, '%.18f\r\n', v(:,size+1-i));
    fclose(fid_v);
end