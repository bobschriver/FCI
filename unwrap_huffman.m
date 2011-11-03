function [ tempImgArray ] = unwrap_huffman( huffman_codestream, dictionary )

%Convert to binary and unwrap
str = dec2bin(huffman_codestream)';
str = str(:)';

tempImgArray = zeros(1, 512^2);

codes = {dictionary.code};


i=0;
pos = 1;

h = waitbar(0, 'Progress');

while i <= 512^2
    i = i+1;
    if mod(i, 500) == 0
        waitbar(i/512^2, h);
    end
    
    numPossibles = inf;
    n=0;
    while numPossibles > 1
        n=n+1;
        possibles = strncmp(str(pos:end), codes, n);
        numPossibles = sum(possibles);
    end

    if numPossibles == 1
        tempImgArray(i) = dictionary(possibles).count;
    else
        tempImgArray(i) = 0;
        print('Oops');
    end
        

    pos = pos+n+1;
end








% for i = 1:256
%     codeLengthVec(i) = numel(dictionary(i).code);
% end
% 
% [~,order] = sort(codeLengthVec, 'descend')
%     
% 
% for i = order
%     ind = strfind(str,dictionary(i).code);
%     tempImgArray(ind) =  dictionary(i).count;
%     str = strrep(str, dictionary(i).code, '');
% end
% 
% clearZeros = tempImgArray(tempImgArray ~= -1);
% 
% img = reshape(clearZeros, 512, 512);

end

