function [ PSNR ] = psnr( im1, im2, maxVal )
%PSNR Calculates peak SNR between two images of the same size

imSize = size(im1);

MSE = ( 1 ./ ( imSize(1)*imSize(2) ) ) * sum( (im1(:) - im2(:)).^2 );

PSNR = 10 * log10(maxVal.^2 / MSE);

end

