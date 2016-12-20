var menu = document.querySelector('.menu');
var menubtn = document.querySelector('.menu-btn');

var manuelbtn = document.querySelector('.profile-btn');
var manuel = document.querySelector('.profile');

var philipbtn = document.querySelector('.philipbtn');
var philip = document.querySelector('.profile2');

var scottbtn = document.querySelector('.scottbtn');
var scott = document.querySelector('.profile3');

var gemmabtn = document.querySelector('.gemmabtn');
var gemma = document.querySelector('.profile4');

var tjbtn = document.querySelector('.tjbtn');
var tj = document.querySelector('.profile5');

var keerbtn = document.querySelector('.keerbtn');
var keer = document.querySelector('.profile6');

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

scottbtn.addEventListener('click', function(d) {
    d.preventDefault();
    scottbtn.classList.toggle('is-active');
    scott.classList.toggle('is-active');
});

scott.addEventListener('click', function(d) {
    if (d.target.classList.contains('is-active')) {
        scottbtn.classList.toggle('is-active');
        scott.classList.toggle('is-active');
    };
});

gemmabtn.addEventListener('click', function(e) {
    e.preventDefault();
    gemmabtn.classList.toggle('is-active');
    gemma.classList.toggle('is-active');
});

gemma.addEventListener('click', function(e) {
    if (e.target.classList.contains('is-active')) {
        gemmabtn.classList.toggle('is-active');
        gemma.classList.toggle('is-active');
    };
});

tjbtn.addEventListener('click', function(f) {
    f.preventDefault();
    tjbtn.classList.toggle('is-active');
    tj.classList.toggle('is-active');
});

tj.addEventListener('click', function(f) {
    if (f.target.classList.contains('is-active')) {
        tjbtn.classList.toggle('is-active');
        tj.classList.toggle('is-active');
    };
});

keerbtn.addEventListener('click', function(g) {
    g.preventDefault();
    keerbtn.classList.toggle('is-active');
    keer.classList.toggle('is-active');
});

keer.addEventListener('click', function(g) {
    if (g.target.classList.contains('is-active')) {
        keerbtn.classList.toggle('is-active');
        keer.classList.toggle('is-active');
    };
});
