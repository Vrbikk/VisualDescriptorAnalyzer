# VisualDescriptorAnalyzer
experimental school project for testing various visual descriptors and their modifications

results:
https://docs.google.com/spreadsheets/d/1cPP9JLb5lNUe4mZr0nRmQLUyEiAX7u9K9Wk-CiyTfmI/edit?usp=sharing

application run:

-install openCV
> cmake CMakeLists.txt
> make
setup config.txt (paths etc..)
chmod u+x run.sh
./run.sh

console output if job_mode:0 -> normal mode

/home/vrbik/CloudStation/=FAV=/BP/playground/Debug/playground config.txt
02-11-2016 12:49:31:  INFO - Collecting images
02-11-2016 12:49:31:  INFO - Collected training data: 375
02-11-2016 12:49:31:  INFO - Collected testing data: 50
02-11-2016 12:49:31:  INFO - Application started in [NORMAL MODE]
02-11-2016 12:49:31:  INFO - ------ config ------

				data_config:[test_folder:test-50/, train_folder:train-50/]
				preprocessing_config:[equalize_hist:0, gaussina_blur:0]
				classification_threads:3
				comparison_method:1

02-11-2016 12:49:31:  INFO - Preprocessing images
>>> Training data preprocessing status: 375/375
>>> Testing data preprocessing status: 50/50
02-11-2016 12:49:31:  INFO - Extracting vectors
>>> Training data extraction status: 375/375
>>> Testing data extraction status: 50/50
02-11-2016 12:49:32:  INFO - Classification started
>>> Classification status: 50/50
02-11-2016 12:49:32:  INFO - ↑ accuracy: <0.340000> ↑
02-11-2016 12:49:32:  INFO - Application ended correctly...