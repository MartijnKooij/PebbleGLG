var maxAppMessageTries = 3;
var appMessageRetryTimeout = 3000;
var appMessageTimeout = 100;
var appMessageQueue = [];

//Note to self, never ever change the order of these data arrays, indices are used as id's
var GLGQuantityData = {
    data: [
        {
            label: "items",
            singularLabel: "item",
            leftDisplayValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            leftValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            rightDisplayValues: ["-", "¼", "⅓", "½", "⅔", "¾"],
            rightValues: [0, 0.25, 0.33, 0.50, 0.66, 0.75]
        },
        {
            label: "grams",
            singularLabel: "gram",
            leftDisplayValues: ["", "1", "2", "3", "4", "5", "6", "7", "8", "9"],
            leftValues: [0, 100, 200, 300, 400, 500, 600, 700, 800, 900],
            rightDisplayValues: ["00", "25", "50", "75"],
            rightValues: [0, 25, 50, 75]
        },
        {
            label: "pounds",
            singularLabel: "pound",
            leftDisplayValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            leftValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            rightDisplayValues: ["-", "¼", "½", "¾"],
            rightValues: [0, 0.25, 0.50, 0.75]
        },
        {
            label: "kilos",
            singularLabel: "kilo",
            leftDisplayValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            leftValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            rightDisplayValues: ["-", "¼", "½", "¾"],
            rightValues: [0, 0.25, 0.50, 0.75]
        },
        {
            label: "milliliters",
            singularLabel: "milliliter",
            leftDisplayValues: ["", "1", "2", "3", "4", "5", "6", "7", "8", "9"],
            leftValues: [0, 100, 200, 300, 400, 500, 600, 700, 800, 900],
            rightDisplayValues: ["00", "25", "50", "75"],
            rightValues: [0, 25, 50, 75]
        },
        {
            label: "litres",
            singularLabel: "litre",
            leftDisplayValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            leftValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            rightDisplayValues: ["-", "¼", "½", "¾"],
            rightValues: [0, 0.25, 0.50, 0.75]
        },
        {
            label: "gallons",
            singularLabel: "gallon",
            leftDisplayValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            leftValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            rightDisplayValues: ["-", "¼", "½", "¾"],
            rightValues: [0, 0.25, 0.50, 0.75]
        },
        {
            label: "ounces",
            singularLabel: "ounce",
            leftDisplayValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            leftValues: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99],
            rightDisplayValues: ["-", "¼", "½", "¾"],
            rightValues: [0, 0.25, 0.50, 0.75]
        }
    ],

    getLabel: function(typeID, value) {
        if (!typeID || typeID < 0 || typeID >= GLGQuantityData.data.length) {typeID = 0;}
        var typeData = GLGQuantityData.data[typeID];
        var result = "";
        var numerator, whole, fraction;
        var singularLabel = (value <= 1);

        if (GLGQuantityData.decimalPlaces(value) === 0) {
            result = value;
        } else {
            if (Math.abs(GLGQuantityData.roundNumber(value % (1/3) - 0.33, 2)) <= 0.05) {
                numerator = Math.floor(value / 0.33);
                whole = Math.floor(numerator / 3);
                fraction = "";
                if (numerator % 3 == 1) {fraction = "1/3";}
                if (numerator % 3 == 2) {fraction = "2/3";}
                if (numerator % 3 == 3) {whole += 1;}
                whole += " ";
                if (whole === 0) {
                    whole = "";
                }
                result = whole + fraction;          
            } else {
                value = GLGQuantityData.roundNumber(value, 5);
                result = GLGQuantityData.decimalToFraction(value);
            }
        }
        
        if (result !== "") {
            if (singularLabel) {
                result += " " + typeData.singularLabel;             
            } else {
                result += " " + typeData.label;
            }
        }
        return  result;
    },

    roundNumber: function(num, dec) {
        var result = Math.round(num*Math.pow(10,dec))/Math.pow(10,dec);
        return result;
    },
    
    decimalToFraction: function(DtF) {
        var i, gcf,
        whole = 0,
        n = GLGQuantityData.decimalPlaces(DtF),
        m = Math.pow(10,n),
        numerator = Math.round(DtF * m),
        negative = numerator < 0,
        denominator = 1 * (numerator !== 0 ? m : 1);
        if (numerator === 0) {
            return "0";
        }
        if (negative) {
            numerator *= -1;
        }
        for (i = (numerator > denominator ? numerator: denominator); i > 0; i--) {
            if ((numerator % i === 0) && (denominator % i === 0)) {
                gcf = i;
                break;
            }
        }
        numerator = numerator / gcf;
        denominator = denominator / gcf;
        if (numerator >= denominator) {
            whole = Math.floor(numerator / denominator);
            numerator = numerator % denominator;
        }
        return (negative ? "-" : "") + (whole > 0 ? whole + " " : "") + (numerator > 0 ? numerator + "/" + denominator : "");
    },
    
    decimalPlaces: function (num) {
        if (!num) {return 0;}
        var decimalSeparator = '.',
        tmp = num.toString(),
        idx = tmp.indexOf(decimalSeparator);
        if (idx >= 0) {
            return (tmp.length - idx - 1);
        }
        return 0;
    }
};
Pebble.addEventListener("ready",
    function(e) {
        var accessKey = localStorage.GLGAccessKey;
        if (!accessKey) {accessKey = "";}

        Pebble.sendAppMessage({"accessKey": accessKey});
    }
);

Pebble.addEventListener("appmessage",
    function(e) {
        if (!localStorage.GLGAccessKey || localStorage.GLGAccessKey === "") {
            return;
        }
        var data;
        var req = new XMLHttpRequest();
        req.open('GET', "http://www.grocerylistgenerator.com/GetJSONListHandler.ashx?accesskey=" + localStorage.GLGAccessKey, true);
        req.onload = function(e) {
            if (req.readyState == 4) {
                if(req.status == 200) {
                    data = JSON.parse(req.responseText.replace("^", " "));
                    var length = data.length;
                    if (length > 0) {
                        data.sort(sortStoreIndex);
                        appMessageQueue.push({"message": {"listSize": length}});
                        for (var i = 0; i < length; i++) {
                            appMessageQueue.push({"message": {"item": data[i].tst.toUpperCase() + '^' + data[i].tnm + '^' + data[i].inm + '^' + GLGQuantityData.getLabel(data[i].iqt, data[i].isv) + '^' + data[i].tsi + '^' + data[i].ihi}});
                        }
                        sendAppMessage();
                    } else {
                        Pebble.sendAppMessage({"error": "No list found, have you synced your GLG app?"});
                    }
                } else {
                    Pebble.sendAppMessage({"error": "Request failed, please try again."});
                }
            }
        };
        req.send(null);
    }
);

Pebble.addEventListener("showConfiguration",
    function() {
        var accessKey = localStorage.GLGAccessKey;
        if (!accessKey) {accessKey = "";}
        Pebble.openURL('http://www.grocerylistgenerator.com/GLGPebbleSettings.html?accesskey=' + accessKey);
    }
);

Pebble.addEventListener("webviewclosed", 
    function(e) {
        var options = JSON.parse(decodeURIComponent(e.response));
        localStorage.GLGAccessKey = options.glgkey;

        Pebble.sendAppMessage({"accessKey": options.glgkey});
    }
);

function sendAppMessage() {
    if (appMessageQueue.length > 0) {
        appMessageQueue[0].numTries = appMessageQueue[0].numTries || 0;
        appMessageQueue[0].transactionId = appMessageQueue[0].transactionId || -1;
        if (appMessageQueue[0].numTries < maxAppMessageTries) {
            Pebble.sendAppMessage(
                appMessageQueue[0].message,
                function(e) {
                    appMessageQueue.shift();
                    setTimeout(function() {
                        sendAppMessage();
                    }, appMessageTimeout);
                },
                function(e) {
                    console.log('Failed sending AppMessage for transactionId:' + e.data.transactionId + '. Error: ' + e.data.error.message);
                    appMessageQueue[0].transactionId = e.data.transactionId;
                    appMessageQueue[0].numTries++;
                    setTimeout(function() {
                        sendAppMessage();
                    }, appMessageRetryTimeout);
                }
            );
        } else {
            console.log('Failed sending AppMessage for transactionId:' + appMessageQueue[0].transactionId + '. Bailing. ' + JSON.stringify(appMessageQueue[0].message));
        }
    }
}

function sortStoreIndex(a,b) {
    if (a.tsi == b.tsi) {
        if (a.inm < b.inm)
            return -1;
        if (a.inm > b.inm)
            return 1;
    } else {
        if (a.tsi < b.tsi)
            return -1;
        if (a.tsi > b.tsi)
            return 1;
    }
    return 0;
}