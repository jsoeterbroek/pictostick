//const pictogrid = document.querySelector(".pictogrid");
//const activitylist = document.querySelector(".activitylist");
const items = [];
var activity_id= '0';
var picto_id= '0';
var list_id= '0';

new Sortable(pictogrid, {
  group: {
      name: 'shared',
      pull: 'clone',
      put: false // Do not allow items to be put into this list
  },
  animation: 150,
  ghostClass: 'blue-background-class',
  sort: false // To disable sorting: set sort to false
});

Sortable.create(activitylist, {
  group: {
    name: 'shared',
    pull: 'clone'
  },
  animation: 150,
  ghostClass: 'blue-background-class',
  onAdd: function (evt) {
    const target = evt.item.querySelector('[pictid]');
    //console.log(target);
    picto_id = $(target).attr('pictid');
    //console.log(activity_id);
    get_assign(picto_id, list_id);
  },
  onUpdate: function (evt) {
    const target = evt.item.querySelector('[pictid]');
    picto_id = $(target).attr('pictid');
    get_assign(picto_id, list_id);
  }
});

/* new Sortable(Monday, {
  group: {
    name: 'shared',
    pull: 'clone'
  },
  animation: 150,
  ghostClass: 'blue-background-class',
  onUpdate: function (evt) {
    get_assign(activity_id, list_id);
  }
});

new Sortable(Tuesday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150,
  ghostClass: 'blue-background-class',
  onUpdate: function (evt) {
    get_assign(activity_id, list_id);
  }
});
new Sortable(Wednesday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150,
  ghostClass: 'blue-background-class',
  onUpdate: function (evt) {
    get_assign(activity_id, list_id);
  }
});
new Sortable(Thursday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150,
  ghostClass: 'blue-background-class',
  onUpdate: function (evt) {
    get_assign(activity_id, list_id);
  }
});
new Sortable(Friday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150,
  ghostClass: 'blue-background-class',
  onUpdate: function (evt) {
    get_assign(activity_id, list_id);
  }
});
new Sortable(Saturday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150,
  ghostClass: 'blue-background-class',
  onUpdate: function (evt) {
    get_assign(activity_id, list_id);
  }
});
new Sortable(Sunday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150,
  ghostClass: 'blue-background-class',
  onUpdate: function (evt) {
    get_assign(activity_id, list_id);
  }
}); */

function get_assign(picto_id, list_id){
  console.log('get_assign triggered');
  console.log(picto_id, list_id);
  //send_request(picto_id, list_id);
}

function send_request(picto_id, list_id){
  $.ajax({
    type: 'GET',
    url: `/myapp/task-assign/${list_id}/${activity_id}/`,
    failure: function(data){
      console.log('failure');
      console.log(data);
    },
  });
}
// Inspired by https://github.com/TomIsLoading/drag-and-drop-kanban
// Inspired by https://medium.com/@jakubjadczak02/drag-drop-and-django-5a5743913c8f
