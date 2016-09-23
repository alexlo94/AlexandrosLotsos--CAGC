var countdown_timer;
var clap_score;
var game_start;

function setup() {
  createCanvas(800,800);
  fill(255,0,0);
  frameRate(30);
  textSize(34);
  
  game_start = false;
  
  countdown_timer = 10;
  clap_score = 0;
}

function draw() {
  background(255);
  
  if(game_start){
    if(countdown_timer > 0){
      countdown();
      text("Score: " + str(clap_score),10,30);
      text("Time Remaining: " + str(countdown_timer), 10, 80);
    }
    else{
      text("Your score was: " + str(clap_score), 10, 30);
      text("Hit the R key to go again!", 10, 80);
    }
  }
  else{
    text("Hit the S key to start clapping!", 10, 30);
  }
  
  
}

//key handlers for controls
// Q is the primary input that increments the score
// S is used to start the game by raising the game start flag (boolean to true)
// R is used to reset the game once a player has finished
function keyPressed(){
  if(key === 'Q' && countdown_timer > 0 && game_start){
    clap_score++;
  }
  if(key === 'S' && !game_start){
    game_start = true;
  }
  if(key === 'R' && countdown_timer === 0 && game_start){
    game_start = false;
    countdown_timer = 10;
    clap_score = 0;
  }
}

//countdown function
//uses the mod (%) operator to figure out 
function countdown(){
  if(frameCount%30 === 0){
    countdown_timer--;
  }
}