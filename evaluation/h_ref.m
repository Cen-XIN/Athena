clc;
ref = imread('./map/ref.png');
hector_maps = imageDatastore('./map/hector/*.png');

while hasdata(hector_maps)
    img = read(hector_maps);
    score = immse(img, ref);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
hector_maps.reset();

while hasdata(hector_maps)
    img = read(hector_maps);
    score = psnr(img, ref);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
hector_maps.reset();

while hasdata(hector_maps)
    img = read(hector_maps);
    score = ssim(img, ref);
    fprintf('%0.4f\n', score);
end