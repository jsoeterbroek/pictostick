new Sortable(avgrid, {
  group: {
      name: 'shared',
      pull: 'clone',
      put: false // Do not allow items to be put into this list
  },
  animation: 150,
  ghostClass: 'blue-background-class',
  sort: false // To disable sorting: set sort to false
});

new Sortable(fugrid, {
  group: {
      name: 'shared',
      pull: 'clone',
  },
  animation: 150,
  ghostClass: 'blue-background-class',
});


new Sortable(Monday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150, ghostClass: 'blue-background-class'
});
new Sortable(Tuesday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150, ghostClass: 'blue-background-class'
});
new Sortable(Wednesday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150, ghostClass: 'blue-background-class'
});
new Sortable(Thursday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150, ghostClass: 'blue-background-class'
});
new Sortable(Friday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150, ghostClass: 'blue-background-class'
});
new Sortable(Saturday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150, ghostClass: 'blue-background-class'
});
new Sortable(Sunday, {
  group: { name: 'shared', pull: 'clone' },
  animation: 150, ghostClass: 'blue-background-class'
});