clc;
ref = imread('./map/ref.png');
gmapping_maps = imageDatastore('./map/gmapping/*.png');

while hasdata(gmapping_maps)
    img = read(gmapping_maps);
    score = immse(img, ref);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
gmapping_maps.reset();

while hasdata(gmapping_maps)
    img = read(gmapping_maps);
    score = psnr(img, ref);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
gmapping_maps.reset();

while hasdata(gmapping_maps)
    img = read(gmapping_maps);
    score = ssim(img, ref);
    fprintf('%0.4f\n', score);
end