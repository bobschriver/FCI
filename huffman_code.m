function [ byteArray, codeTable ] = huffman_code( img )

% Find the frequency of each pixel value of the image
dataDist = hist(double(img(:)), 0:1:255);

% Sort descending, and establish paired digital counts (location in array)
[freq, count] = sort(dataDist, 'descend');
count = count-1;

% Calculate probability from frequency
prob = freq/numel(img);

% Pass the probabilities and matched digital counts to the tree generator
codeTable = split_tree('', prob, count);

% Re-sort so that indices line up with digital counts (runtime efficiency)
[~, sortOrder] = sort([codeTable.count]);
codeTableFinal = codeTable(sortOrder);

% Concanenate the codes for each pixel (image treated as a column vector)
% Add 1 to move 0-255 to 1-256
strRep = [codeTableFinal(img(:)+1).code];

% find the number of needed bytes
numBytes = ceil(numel(strRep)/8.0);

% preconstruct byte array
byteArray = zeros(1, numBytes, 'uint8');

% Convert every 8 bits to uint8's
for i = 1:numBytes-1
    byteArray(i) = bin2dec(strRep(((8*(i-1))+1):((8*(i-1))+8)));
end

% Deal with the uneven byte
lastByte = strRep(8*(numBytes-1)+1:end);
numBits = numel(lastByte);
if numBits ~= 0
    for i = 1:(8-numBits)
        lastByte = [lastByte, '0'];
    end
end
byteArray(end) = bin2dec(lastByte);





end

