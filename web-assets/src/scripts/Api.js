export class Api {
  /**
   * @param {Object} options
   */
  constructor(options) {
    this.getStateTimeout = options.getStateTimeout;
  }

  /**
   * @public
   * @returns {Promise<Object>}
   */
  async getState() {
    const url = '/state';

    const controller = new AbortController();
    const fetchTimeout = setTimeout(() => controller.abort(), this.getStateTimeout);

    const response = await fetch(url, {signal: controller.signal});
    clearTimeout(fetchTimeout);

    if (!response.ok) {
      throw response;
    }

    const json = await response.json();

    console.log('getState', url, json);

    return json;
  }

  /**
   * @public
   * @param {string} command
   * @param {Object} [parameters={}]
   * @returns {Promise<void>}
   */
  async postCommand(command, parameters = {}) {
    const urlSearchParams = new URLSearchParams(parameters);
    urlSearchParams.set('command', command);
    const url = `/command?${urlSearchParams.toString()}`;

    console.log('postCommand', url);

    const response = await fetch(url, {method: 'post'});

    if (!response.ok) {
      throw response;
    }
  };
}
