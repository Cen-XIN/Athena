clc;
cartographer_maps = imageDatastore('./map/cartographer/*.png');

while hasdata(cartographer_maps)
    img = read(cartographer_maps);
    score = brisque(img);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
cartographer_maps.reset();

while hasdata(cartographer_maps)
    img = read(cartographer_maps);
    score = niqe(img);
    fprintf('%0.4f\n', score);
end

fprintf('\n');
cartographer_maps.reset();

while hasdata(cartographer_maps)
    img = read(cartographer_maps);
    score = piqe(img);
    fprintf('%0.4f\n', score);
end