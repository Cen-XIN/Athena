clc;
ref = imread('./map/ref.png');
cartographer_maps = imageDatastore('./map/cartographer/*.png');

while hasdata(cartographer_maps)
    img = read(cartographer_maps);
    score = immse(img, ref);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
cartographer_maps.reset();

while hasdata(cartographer_maps)
    img = read(cartographer_maps);
    score = psnr(img, ref);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
cartographer_maps.reset();

while hasdata(cartographer_maps)
    img = read(cartographer_maps);
    score = ssim(img, ref);
    fprintf('%0.4f\n', score);
end