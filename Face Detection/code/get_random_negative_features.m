% Starter code prepared by James Hays for CS 143, Brown University
% This function should return negative training examples (non-faces) from
% any images in 'non_face_scn_path'. Images should be converted to
% grayscale, because the positive training data is only available in
% grayscale. For best performance, you should sample random negative
% examples at multiple scales.

function features_neg = get_random_negative_features(non_face_scn_path, feature_params, num_samples)
% 'non_face_scn_path' is a string. This directory contains many images
%   which have no faces in them.
% 'feature_params' is a struct, with fields
%   feature_params.template_size (probably 36), the number of pixels
%      spanned by each train / test template and
%   feature_params.hog_cell_size (default 6), the number of pixels in each
%      HoG cell. template size should be evenly divisible by hog_cell_size.
%      Smaller HoG cell sizes tend to work better, but they make things
%      slower because the feature dimensionality increases and more
%      importantly the step size of the classifier decreases at test time.
% 'num_samples' is the number of random negatives to be mined, it's not
%   important for the function to find exactly 'num_samples' non-face
%   features, e.g. you might try to sample some number from each image, but
%   some images might be too small to find enough.

% 'features_neg' is N by D matrix where N is the number of non-faces and D
% is the template dimensionality, which would be
%   (feature_params.template_size / feature_params.hog_cell_size)^2 * 31
% if you're using the default vl_hog parameters

% Useful functions:
% vl_hog, HOG = VL_HOG(IM, CELLSIZE)
%  http://www.vlfeat.org/matlab/vl_hog.html  (API)
%  http://www.vlfeat.org/overview/hog.html   (Tutorial)
% rgb2gray

image_files = dir( fullfile( non_face_scn_path, '*.jpg' ));
num_images = length(image_files);
num_samples_per_img = ceil(num_samples/num_images);

D = (feature_params.template_size / feature_params.hog_cell_size)^2 * 31;
features_neg = zeros(num_samples, D);

% For best performance, you should sample random negative examples at multiple scales.
for i = 1:num_images

	img_path = fullfile( non_face_scn_path, image_files(i).name );
	img = imread(img_path);

	if size(img, 3) == 3
	    img = rgb2gray(img);
	end

	[height, width] = size(img);

	for j = 1:num_samples_per_img
		
		top_left_height = ceil( rand()*(height-feature_params.template_size) );
		top_left_width = ceil( rand()*(width-feature_params.template_size) );

		img_crop = img(top_left_height:top_left_height+feature_params.template_size-1, top_left_width:top_left_width+feature_params.template_size-1);

		index = (i-1)*num_samples_per_img + j;

		hog = vl_hog(single(img_crop), feature_params.hog_cell_size);
		features_neg(index, 1:end) = reshape(hog, 1, D);

	end
end


