function [ img ] = ycc2rgb( Y, Cb, Cr )
% User must input Y, Cb, Cr separatley

% Increase percission of separated data.
% Eliminating computational error.
Y = double(Y);
Cr = double(Cr);
Cb = double(Cb);

% Convert Y, Cr, Cb image data to RGB color space.
R = Y + 1.4022.*(Cr - .5);
G = Y - 0.34414.*(Cb - .5) - 0.71414.*(Cr - .5);
B = Y + 1.772.* (Cb - .5);

% Combine separate RGB channels into one image.
img(:,:,1) = R;
img(:,:,2) = G;
img(:,:,3) = B;

end
