function [ coeff, vars ] = im2func( im, blockSize )
%IM2FUNC Find 2D function of best fit for each region in an image
%   Uses curve fitting to find a 2D function of best for for each region of
%   an image. Requires specification of block size and assumes input image
%   is scaled from 0-255.

%% Assume input is 0-255, rescale to 0-1
im = im./255;

%% Split the image into regions
[rows, cols] = size(im);

%Number of blocks the image can be divided into
numWidth = rows / blockSize(1);
numHeight = cols / blockSize(2);
if mod(cols,numWidth)~=0 || mod(rows,numWidth)~=0
    error('Non-optimal block size selected.');
end

blocks = mat2cell( im, repmat(blockSize(1),1,numWidth),...
    repmat(blockSize(2),1,numHeight) );

%% Perform curve fitting on each region
[x, y] = ind2sub( blockSize, 1:numel(blocks{1}) );
fitObj = cell(numHeight, numWidth);

%Fit a surface to each block using 3rd order polynomial
parfor n=1:numel(blocks)
    fitObj{n} = fit( [x',y'], blocks{n}(:), 'poly33', 'Normalize','off' );
end

%% Get coeffcients and variable names
nameFun = @(c) coeffnames(c);
coeffFun = @(c) coeffvalues(c);

vars = cellfun(nameFun, fitObj, 'UniformOutput',0);
coeff = cellfun(coeffFun, fitObj, 'UniformOutput',0);
end