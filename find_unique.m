function [ unique_patches , counts ] = find_unique( image , patch_size)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

unique_patches = zeros(0 , patch_size(1) , patch_size(2));
counts = zeros(0 , 0)


for y = 1:patch_size(2):size(image , 2)
    for x = 1:patch_size(1):size(image , 1)
        y_to = y + (patch_size(2) - 1);
        x_to = x + (patch_size(1) - 1);
        
        if x_to > size(image , 1)
            break
        end
        
        if y_to > size(image , 2)
            break
        end
        
        patch = image(x:x_to , y:y_to);
        
        %unique_patches(1 , 1:patch_size , 1:patch_size) = patch
        
        %c = unique_patches(: , 1:patch_size , 1:patch_size);
       
        
        %index = find(c == patch)
        
        index = 0;
        for i = 1:size(unique_patches , 1)
            
            if isequal(reshape(unique_patches(i , : , :) , patch_size(1) , patch_size(2)) , patch)
                index = i
                break;
            end
        end
        
        if index == 0
            unique_patches(end + 1 , 1:patch_size(1) , 1:patch_size(2)) = patch;
            index = size(unique_patches , 1);
            counts(1 , end + 1) = 0;
        
        end
        
        counts(1 , index) = counts(1 , index) + 1;
        
    end
end



end

