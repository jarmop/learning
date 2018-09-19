/**
 * Custom promise implementation.
 */
class MyPromise {
  constructor(task) {
    this.thenCallback = null;
    this.catchCallback = null;
    this.finallyCallback = null;
    this.isResolved = false;

    let resolve = (value) => {
      this.isResolved = true;
      this.resolvedValue = value;
      if (this.thenCallback) {
        this.thenCallback(value);
      }
      if (this.finallyCallback) {
        this.finallyCallback(value);
      }
    };
    let reject = (value) => {
      this.isRejected = true;
      this.rejectedValue = value;
      if (this.catchCallback) {
        this.catchCallback(value);
      }
      if (this.finallyCallback) {
        this.finallyCallback(value);
      }
    };

    task(resolve, reject);
  }

  /**
   * @param callback
   * @returns {MyPromise}
   */
  then(callback) {
    if (this.isResolved) {
      callback(this.resolvedValue);
    } else {
      this.thenCallback = callback;
    }

    return this;
  }

  /**
   * @param callback
   * @returns {MyPromise}
   */
  catch(callback) {
    if (this.isRejected) {
      callback(this.rejectedValue);
    } else {
      this.catchCallback = callback;
    }

    return this;
  }

  /**
   * @param callback
   * @returns {MyPromise}
   */
  finally(callback) {
    if (this.isResolved) {
      callback();
    } else if (this.isRejected) {
      callback();
    } else {
      this.finallyCallback = callback;
    }

    return this;
  }
}

// let p = new Promise((resolve, reject) => {
let p = new MyPromise((resolve, reject) => {
  if (true) {
    setTimeout(() => resolve('res'), 1000);
  } else {
    reject('rej');
  }
  // console.log('Resolving nor rejecting won\'t stop function execution.');
});

p.then(a => {
  console.log('then: ' + a);
}).catch(b => {
  console.log('catch: ' + b);
}).finally(() => {
  console.log('finally');
});