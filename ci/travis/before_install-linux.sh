sudo apt-key adv --fetch-keys http://repos.codelite.org/CodeLite.asc
sudo add-apt-repository 'deb http://repos.codelite.org/wx3.0/ubuntu/ precise universe'
sudo apt-get update
sudo apt-get install -y libwxbase3.0-dev libwxgtk3.0-dev
