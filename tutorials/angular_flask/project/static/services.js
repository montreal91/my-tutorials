
angular.module('myApp').factory('AuthService',
    ['$q', '$timeout', '$http',
    function ($q, $timeout, $http) {
        var user = null;

        // return available functions for use in controllers
        return ({
            isLoggedIn: isLoggedIn,
            login: login,
            logout: logout,
            register: register
        });

    function isLoggedIn() {
        if (user) {
            return true;
        } else {
            return false;
        }
    }

    function login(email, password) {
        // create a new instance of deferred
        var deferred = $q.defer();

        // send a post request to the server
        $http.post('/api/login/', {email: email, password: password})
        // handle success
        .success(function (data, status) {
            if (status === 200 && data.result) {
                user = true;
                deferred.resolve();
            } else {
                user = false;
                deferred.reject();
            }
        })
        // handle error
        .error( function (data) {
            user = false;
            deferred.reject();
        });
        return deferred.promise;
    }

    function logout() {
        var deferred = $q.defer();

        // send a get request to the server
        $http.get('/api/logout/')
        // handle success
        .success(function (data) {
            user = false;
            deferred.resolve();
        })
        // handle error
        .error( function (data) {
            user = false;
            deferred.reject();
        });

        // return promise object
        return deferred.promise;
    }

    function register(email, password) {
        var deferred = $q.defer();

        $http.post('/api/register/', {email: email, password: password})
        .success(function (data, status) {
            if (status === 200 && data.result) {
                deferred.resolve();
            } else {
                deferred.reject();
            }
        })
        .error( function (data) {
            deferred.reject();
        });
        return deferred.promise;
    }
}]);
