var menu = document.querySelector('.menu');
var menubtn = document.querySelector('.menu-btn');

var manuelbtn = document.querySelector('.profile-btn');
var manuel = document.querySelector('.profile');

var philipbtn = document.querySelector('.philipbtn');
var philip = document.querySelector('.profile2');

menubtn.addEventListener('click', function(a) {
    a.preventDefault();
    menubtn.classList.toggle('is-active');
    menu.classList.toggle('is-active');
});

menu.addEventListener('click', function(a) {
    if (a.target.classList.contains('is-active')) {
        menubtn.classList.toggle('is-active');
        menu.classList.toggle('is-active');
    };
});

manuelbtn.addEventListener('click', function(b) {
    b.preventDefault();
    manuelbtn.classList.toggle('is-active');
    manuel.classList.toggle('is-active');
});

manuel.addEventListener('click', function(b) {
    if (b.target.classList.contains('is-active')) {
        manuelbtn.classList.toggle('is-active');
        manuel.classList.toggle('is-active');
    };
});

philipbtn.addEventListener('click', function(c) {
    c.preventDefault();
    philipbtn.classList.toggle('is-active');
    philip.classList.toggle('is-active');
});

philip.addEventListener('click', function(c) {
    if (c.target.classList.contains('is-active')) {
        philipbtn.classList.toggle('is-active');
        philip.classList.toggle('is-active');
    };
});
