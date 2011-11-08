function [ filepath ] = write_fci( imageheight, imagewidth, imagesize,...
                                   scaleFactor, frequencyThreshold,...
                                   yFuncRep, CrFuncRep, CbFuncRep,...
                                   huffman1, huffman2)
% Filename set standard to Image.fci, 
% Open writable and readable file.
fid = fopen('Image.fci', 'w+');

% Signature "cookie"
fwrite(fid , 'FC', 'uint16');

%Write image height in pixels
fwrite(fid , 'imageheight', 'int16');

%Write image width in pixels
fwrite(fid, 'imagewidth', 'int16');

%Write image size
fwrite(fid, 'imagesize');

%Write sclae factor
fwrite(fid, 'scaleFactor');

%Write frequency threshold
fwrite(fid,'frequencyThreshold');

%Write Ychannel Function rep
fwrite(fid, 'YFuncRep');

%Write Crchannel Function rep
fwrite(fid, 'CrFuncRep');

%Write Cbchannel Function rep
fwrite(fid, 'CbFuncRep');

%Write first set of huffman coefficients
fwrite(fid, 'huffman1');

%Write second set of huffman coefficients
fwrite(fid, 'huffman2');

%when finished close file ID
fclose(fid);

end


