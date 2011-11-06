function [ im ] = func2im( coeff, blockSize )
%FUNC2IM Reconstruct image using 3rd order polynomial given coeffcient values

%% Initialize
%Generate x and y coodinates for each block
[x, y] = ind2sub( blockSize, 1:(blockSize(1)*blockSize(2)) );
value = cell( size(coeff) );

%% Reconstruct each block
parfor n=1:numel(coeff)
%     value{n} = coeff{n}(1) + coeff{n}(2).*x + coeff{n}(3).*y +...
%         coeff{n}(4).*x.^2 + coeff{n}(3).*x.*y + coeff{n}(4).*y.^2;
    value{n} = coeff{n}(1) + coeff{n}(2).*x + coeff{n}(3).*y +...
        coeff{n}(4).*x.^2 + coeff{n}(5).*x.*y + coeff{n}(6).*y.^2 +...
        coeff{n}(7).*x.^3 + coeff{n}(8).*x.^2.*y + coeff{n}(9).*x.*y.^2 +...
        coeff{n}(10).*y.^3;
end

%% Reassemble image from blocks
reshaped = cellfun(@(x) reshape(x, blockSize), value, 'UniformOutput', 0);
im = cell2mat(reshaped);

end

