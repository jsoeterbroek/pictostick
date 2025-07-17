# PictoStick Todo List

Based on GitHub issues from https://github.com/jsoeterbroek/pictostick/issues

## Unchecked Items 🔲

### Create function to reset all activities to undone (issue #16) ✅


### Limit web interface max 20 activities (issue #20) 🔲
### Activity list for the whole week (issue #21) ✅
- At the moment we have one activities list for one day.
- This activities list is configurable with javascript in data/index.html
- Results are saved in data/data.json
- We need to refactor code to make one list of activities per day of the week, so 7 in total, using separate files for each day (e.g., data/data_monday.json).
- This will allow the users to have different lists of activities for each day, for
  instance each day has common picto's, but thursday afternoon (after lunch) is 'chess playing' 
  activity and each monday morning (before lunch) has 'basketball' activity.
- Start with creating data/data_week.json file first and stop there to review ✅
- Then refactor the relevant code to use 7 separate daily files ✅ 


## Completed Items ✅

### esp wifimanager timezone (issue #18) ✅
### Create Display color themes (issue #19) ✅


## Notes

- Each item should reference a GitHub issue number when available
- Plans will be posted as comments on the corresponding GitHub issues before implementation
