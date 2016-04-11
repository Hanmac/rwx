codename=`lsb_release -s -c`
sudo apt-key adv --fetch-keys http://repos.codelite.org/CodeLite.asc

if [ ${codename} = 'trusty' ]; then
sudo apt-add-repository 'deb http://repos.codelite.org/wx3.0.2/ubuntu/ trusty universe'
else
sudo add-apt-repository 'deb http://repos.codelite.org/wx3.0/ubuntu/ precise universe'
fi
sudo apt-get update
sudo apt-get install -y libwxbase3.0-dev libwxgtk3.0-dev libgtk2.0-dev
