require 'sinatra'

set :public_folder, 'public'

get '/' do
    erb :onoff
end