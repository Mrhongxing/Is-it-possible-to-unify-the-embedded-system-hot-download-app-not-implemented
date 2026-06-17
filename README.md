I’m an undergrad who knows both front-end and back-end. When I got to learning Flash writing, I was thinking, is it possible to treat library functions like GPIO as dependencies or drivers, run software as a hot update by jumping to execute the app, and then control the app’s writing and usage like you would on a Linux command line?

Right now, the way I can implement it is to write the 'jump' jump function to a fixed location (to avoid offset issues), but each library function only keeps the part that’s used during compilation. If I import it as a module, it ends up wasting a lot of space, and using a dynamic calculation method would require doing the calculation at write time.

Right now, I'm thinking about whether this is feasible, and on an embedded system focused on minimalism, it goes against the original intention. On a traditional disposable embedded system, does it make sense to package features as an app like this?

Humbly asking kind tech gurus for guidance
