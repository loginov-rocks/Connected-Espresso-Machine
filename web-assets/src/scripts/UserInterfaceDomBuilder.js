export class UserInterfaceDomBuilder {
  /**
   * @public
   * @param {HTMLElement} element
   * @returns {void}
   */
  build(element) {
    element.innerHTML = `<div class="row">
        <div class="col">
          <h1>Connected Espresso Machine</h1>
        </div>
      </div>

      <div class="row">
        <div class="col">
          Toggle
        </div>
        <div class="col">
          <span class="toggleState" id="toggleMakeSteamState">Make Steam</span>
        </div>
        <div class="col">
          <span class="toggleState" id="toggleOffState">Off</span>
        </div>
        <div class="col">
          <span class="toggleState" id="toggleBoilState">Boil</span>
        </div>
        <div class="col">
          <span class="toggleState" id="togglePourWaterState">Pour Water</span>
        </div>
      </div>

      <div class="row">
        <div class="col">
          <span class="commandState" id="commandState">Unknown</span>
        </div>
        <div class="col">
          <span class="doneState" id="doneState">Done</span>
        </div>
        <div class="col">
          <input disabled id="off" type="button" value="Off" />
        </div>
      </div>

      <div class="row">
        <div class="col">
          <span class="boilerState" id="boilerState">Boiler</span>
          <input disabled id="coolDown" type="button" value="Cool Down" />
          <input disabled id="boil" type="button" value="Boil" />
          <input disabled id="makeSteam" type="button" value="Make Steam" />
        </div>
        <div class="col">
          <span class="pumpState" id="pumpState">Pump</span>
          <input disabled id="pourWater" type="button" value="Pour Water" />
          <input disabled id="stopPouringWater" type="button" value="Stop Pouring Water" />
        </div>
      </div>

      <div class="row">
        <div class="col">
          <label class="makeCoffeeSecondsLabel" for="makeCoffeeSeconds" id="makeCoffeeSecondsLabel">30"</label>
          <input disabled id="makeCoffeeSeconds" max="120" min="1" type="range" value="30" />
        </div>
        <div class="col">
          <input disabled id="makeCoffee" type="button" value="Make Coffee" />
        </div>
      </div>

      <div class="row">
        <div class="col">
          <a href="https://github.com/loginov-rocks/Connected-Espresso-Machine" rel="noopener noreferrer"
             target="_blank">GitHub</a>
        </div>
      </div>`;
  }
}
