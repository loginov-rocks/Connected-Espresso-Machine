$(function () {

    var refreshTimer = setTimeout(function refreshFunc() {
        $.getJSON('/api/').done(refresh);
        refreshTimer = setTimeout(refreshFunc, 2000);
    }, 2000);

    $.getJSON('/api/').done(refresh);

    $('a[id]').click(function () {
        var url = '/api/' + $(this).attr('id');
        if ($(this).attr('id') == 'makeCoffee') {
            url += '/' + $('#makeCoffeeSeconds').text()
        }
        url += '/';

        $.getJSON(url)
            .done(refresh)
            .fail(function (jqXHR, textStatus, errorThrown) {
                alert('Ошибка: ' + jqXHR.responseText + ', ' + textStatus);
            });
    });

    $('#makeCoffeeSecondsInput').on('change mousemove', function () {
        $('#makeCoffeeSeconds').text($(this).val());
    });

    function refresh(json) {
        $('.lastCommand').removeClass('lastCommand');
        $('#' + json.lastCommand).addClass('lastCommand');

        if (json.pumpState) {
            $('.currentPump').addClass('working');
        }
        else {
            $('.currentPump').removeClass('working');
        }

        if (json.boilerState) {
            $('.currentBoiler').addClass('working');
        }
        else {
            $('.currentBoiler').removeClass('working');
        }

        var currentButton = '';
        switch (json.boilerStage) {
            case 'steam':
                currentButton = 'makeSteam';
                break;
            case 'hot':
                currentButton = 'heatWater';
                break;
            default:
                currentButton = 'coolDown';
                break;
        }
        $('.boiler .current').removeClass('current');
        $('.boiler #' + currentButton).addClass('current');

        var targetButton = '';
        switch (json.boilerTargetStage) {
            case 'steam':
                targetButton = 'makeSteam';
                break;
            case 'hot':
                targetButton = 'heatWater';
                break;
            default:
                targetButton = 'coolDown';
                break;
        }
        $('.boiler .target').removeClass('target');
        $('.boiler #' + targetButton).addClass('target');
    }

});
