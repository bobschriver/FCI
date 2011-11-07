function [ im ] = func2im( coeff, blockSize )
%FUNC2IM Reconstruct image using a polynomial given coeffcient values
%   Supports 2nd, 3rd, and 4th order polynomial 

%% Initialize
%Set defaults
if ~exist('blockSize','var')
    blockSize = [32 32];
end

%Generate x and y coodinates for each block
[x, y] = ind2sub( blockSize, 1:(blockSize(1)*blockSize(2)) );
value = cell( size(coeff) );

%Determine which equation to use based on number of coeffcients, as this
%indicates the order of the polynominal fitted to the data
numCoeff = numel(coeff{1});

%% Reconstruct each block
switch numCoeff
    case 4
        parfor n=1:numel(coeff)
            value{n} = coeff{n}(1) + coeff{n}(2).*x + coeff{n}(3).*y +...
                coeff{n}(4).*x.^2 + coeff{n}(3).*x.*y + coeff{n}(4).*y.^2;
        end
    case 10
        parfor n=1:numel(coeff)
            value{n} = coeff{n}(1) + coeff{n}(2).*x + coeff{n}(3).*y +...
                coeff{n}(4).*x.^2 + coeff{n}(5).*x.*y +...
                coeff{n}(6).*y.^2 + coeff{n}(7).*x.^3 +...
                coeff{n}(8).*x.^2.*y + coeff{n}(9).*x.*y.^2 +...
                coeff{n}(10).*y.^3;
        end
    case 15
        parfor n=1:numel(coeff)
            value{n} = coeff{n}(1) + coeff{n}(2).*x + coeff{n}(3).*y +...
                coeff{n}(4).*x.^2 + coeff{n}(5).*x.*y +...
                coeff{n}(6).*y.^2 + coeff{n}(7).*x.^3 +...
                coeff{n}(8).*x.^2.*y + coeff{n}(9).*x.*y.^2 +...
                coeff{n}(10).*y.^3 + coeff{n}(11).*x.^4 +...
                coeff{n}(12).*x.^3.*y + coeff{n}(13).*x.^2.*y.^2 +...
                coeff{n}(14).*x.*y.^3 + coeff{n}(15).*y.^4;
        end
end

%% Reassemble image from blocks
reshaped = cellfun(@(x) reshape(x, blockSize), value, 'UniformOutput', 0);
im = cell2mat(reshaped);

end

