function [ output_args ] = compress_img( img )

% Split image into YCrCb channels
[Ychannel, Cbchannel, Crchannel] = rgb2ycc(img);

scaleVal = 50;

% Break the Y channel into low and high frequency bands (scale 50 sets
% where the break occurs)
[Ychannel_LF Ychannel_HF] = frequency_decomp(Ychannel, scaleVal);
[Cbchannel_LF, ~] = frequency_decomp(Cbchannel, scaleVal);
[Crchannel_LF, ~] = frequency_decomp(Crchannel, scaleVal);


% Break image into 32x32 blocks
%Ychannel_LF_func = im2func(Ychannel_LF, [32,32]);
%Cbchannel_LF_func = im2func(Cbchannel_LF, [32 32]);
%Crchannel_LF_func = im2func(Crchannel_LF, [32 32]);


Ychannel_out = Ychannel_LF + Ychannel_HF;
Cbchannel_out = Cbchannel_LF;
Crchannel_out = Crchannel_LF;






end

