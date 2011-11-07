function [ output_args ] = compress_img( img )

% Split image into YCrCb channels
[Ychannel, Crchannel, Cbchannel] = rgb2ycc(img);

% Break the Y channel into low and high frequency bands (scale 50 sets
% where the break occurs)
[Ychannel_LF Ychannel_HF] = frequency_decomp(Ychannel, 50);
[Crchannel_LF Crchannel_HF] = frequency_decomp(Crchannel, 50);
[Cbchannel_LF Cbchannel_HF] = frequency_decomp(Cbchannel, 50);

% Break image into 32x32 blocks
Ychannel_LF_func = im2func(Ychannel_LF, [32,32]);

end

